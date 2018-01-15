#pragma once
#include "collider.h"
#include "quadtree.h"
#include "transform.h"
#include "vector2.h"
#include <algorithm>
#include <memory>
#include <vector>

namespace Mino
{
class ColliderComponent;

class IPhysicsSystem
{
public:
    using Collider = ColliderComponent;
    using World = Quadtree<Collider>;

    virtual ~IPhysicsSystem() {}

    virtual void update() = 0;
    virtual std::shared_ptr<World> getWorld() = 0;
    virtual void add(std::weak_ptr<Collider> const&) = 0;
    virtual void remove(std::weak_ptr<Collider> const&) = 0;
};

class PhysicsSystem : public IPhysicsSystem
{
public:
    static std::shared_ptr<PhysicsSystem> create() { return std::make_shared<PhysicsSystem>(); }

    virtual void update();
    virtual void add(std::weak_ptr<Collider> const&);
    virtual void remove(std::weak_ptr<Collider> const&);

    virtual std::shared_ptr<World> getWorld() { return world; }

protected:
    std::shared_ptr<World> world = std::make_shared<World>();
    std::vector<std::weak_ptr<Collider>> colliders;
};

} // namespace Mino
