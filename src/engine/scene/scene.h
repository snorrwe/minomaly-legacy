#pragma once
#include "core.h"
#include <memory>

namespace Mino
{

class Core;

class Scene
{
public:
    Scene(std::shared_ptr<Core> engine) : engine(engine.get()) {}
    virtual ~Scene() {}

    virtual void update() = 0;
    virtual void start() {}

protected:
    Core* engine;
};

} // namespace Mino
