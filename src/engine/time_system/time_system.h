#pragma once
#include <chrono>
#include <memory>

namespace Mino
{

class EngineCore;

class ITimeSystem
{
public:
    friend class EngineCore;

    typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
    typedef std::chrono::duration<double, std::milli> Milli;

    virtual ~ITimeSystem() {}

    virtual TimePoint now() const = 0;
    virtual Milli deltaTime() const = 0;

protected:
    virtual void update(TimePoint const& now) = 0;
};

class TimeSystem : public ITimeSystem
{
public:
    static std::shared_ptr<TimeSystem> create() { return std::make_shared<TimeSystem>(); }

    virtual ~TimeSystem() {}

    virtual TimePoint now() const { return currentTime; }
    virtual Milli deltaTime() const { return dtime; }

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
