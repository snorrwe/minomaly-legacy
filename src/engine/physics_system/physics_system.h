#pragma once
#include "quadtree.h"
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
    static std::shared_ptr<PhysicsSystem> create() { return std::make_shared<PhysicsSystem>(); }

    virtual void update();
};

} // namespace Mino
