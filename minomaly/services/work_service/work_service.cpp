#include "work_service.h"

using namespace Mino;

WorkService::WorkService(uint8_t concurrency)
{
    if (!concurrency) throw std::invalid_argument("Concurrency must be greater than 0!");

    for (auto i = 0; i < concurrency; ++i)
    {
        threads.emplace_back(&WorkService::worker, this);
    }
}

WorkService::~WorkService()
{
    done = true;
    notify_all();
    threads.join();
}

void WorkService::worker()
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
