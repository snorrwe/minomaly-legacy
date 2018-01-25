#pragma once
#include "p_service.h"

#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <tuple>
#include <vector>

namespace Mino
{
class IJob
{
public:
    virtual ~IJob() {}

    virtual void execute() = 0;
};

template <class TReturn> struct Job : IJob
{
    Job(std::function<TReturn()> job) : job(job) {}
    std::function<TReturn()> job;
    std::promise<TReturn> promise = {};

    virtual void execute() { promise.set_value(job()); }
};

class IWorkService : public IService
{
public:
    virtual ~IWorkService() {}
};

struct Threads : std::vector<std::thread>
{
    void join()
    {
        for (auto i = begin(); i != end(); ++i)
        {
            i->join();
        }
    }
};

class WorkService : public IWorkService, std::mutex, std::condition_variable
{
public:
    using Queue = std::queue<std::unique_ptr<IJob>>;
    using UniqueLock = std::unique_lock<std::mutex>;

    static std::shared_ptr<WorkService> create() { return std::make_shared<WorkService>(); }

    WorkService(uint8_t concurrency = std::thread::hardware_concurrency())
    {
        if (!concurrency) throw std::invalid_argument("Concurrency must be greater than 0!");

        for (auto i = 0; i < concurrency; ++i)
        {
            threads.emplace_back(&WorkService::worker, this);
        }
    }

    virtual ~WorkService()
    {
        done = true;
        notify_all();
        threads.join();
    }

    WorkService(WorkService const&) = delete;
    WorkService(WorkService&&) = delete;
    WorkService& operator=(WorkService const&) = delete;
    WorkService& operator=(WorkService&&) = delete;

    template <typename TReturn> std::future<TReturn> requestWork(std::function<TReturn()> job)
    {
        auto lock = UniqueLock{*this};
        auto addedJob = std::make_unique<Job<TReturn>>(job);
        auto result = addedJob->promise.get_future();
        queue.push(std::move(addedJob));
        notify_one();
        return result;
    }

private:
    void worker()
    {
        auto lock = UniqueLock{*this};
        while (!done)
        {
            if (!queue.empty())
            {
                auto item = std::move(queue.front());
                queue.pop();
                notify_one();
                lock.unlock();
                item->execute();
                lock.lock();
            }
            else
            {
                wait(lock);
            }
        }
    }

    bool done = false;
    Threads threads = {};
    Queue queue = {};
};

} // namespace Mino
