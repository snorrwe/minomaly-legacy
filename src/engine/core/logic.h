#pragma once
#include "core.h"
#include <memory>

class Core;

class ILogic
{
public:
    ILogic(std::shared_ptr<Core> engine) : engine(engine) {}
    virtual ~ILogic() {}

    virtual void update() = 0;
    virtual void start() = 0;

protected:
    std::shared_ptr<Core> engine;
};
