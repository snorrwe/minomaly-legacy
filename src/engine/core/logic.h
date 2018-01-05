#pragma once
#include "core.h"
#include <memory>

class Core;

class ILogicSystem
{
public:
    ILogicSystem(std::shared_ptr<Core> engine) : engine(engine.get()) {}
    virtual ~ILogicSystem() {}

    virtual void update() = 0;
    virtual void start() = 0;

protected:
    Core* engine;
};
