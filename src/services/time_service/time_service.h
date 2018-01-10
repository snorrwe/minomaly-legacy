#pragma once
#include "p_service.h"
#include <chrono>
#include <memory>

namespace Mino
{

class EngineCore;

class ITimeService : public IService
{
public:
    friend class EngineCore;

    typedef std::chrono::duration<double, std::milli> Milli;
    typedef std::chrono::time_point<std::chrono::system_clock, Milli> TimePoint;

    virtual ~ITimeService() {}

    virtual TimePoint now() const = 0;
    virtual double deltaTime() const = 0;

protected:
    virtual void update(TimePoint const& now) = 0;
};

class TimeService : public ITimeService
{
public:
    static std::shared_ptr<TimeService> create() { return std::make_shared<TimeService>(); }

    virtual ~TimeService() {}

    virtual TimePoint now() const { return currentTime; }
    virtual double deltaTime() const { return dtime.count(); }

protected:
    virtual void update(TimePoint const& now)
    {
        dtime = now - currentTime;
        currentTime = now;
    }

private:
    Milli dtime;
    TimePoint currentTime;
};

} // namespace Mino
