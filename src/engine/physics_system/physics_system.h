#pragma once
#include "transform.h"
#include <memory>

namespace Mino
{

class IPhysicsSystem
{
public:
    virtual ~IPhysicsSystem() {}

    virtual void update() = 0;
};

class PhysicsSystem : public IPhysicsSystem
{
public:
    PhysicsSystem();
    virtual ~PhysicsSystem() {}

    virtual void update();
};

} // namespace Mino
