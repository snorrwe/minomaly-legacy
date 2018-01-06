#pragma once
#include "core.h"
#include <memory>

namespace Mino
{

class Core;

class ILogicSystem
{
public:
    ILogicSystem(std::shared_ptr<Core> engine) : engine(engine.get()) {}
    virtual ~ILogicSystem() {}

    virtual void update() = 0;
    virtual void start() {}

protected:
    Core* engine;
};

} // namespace Mino
