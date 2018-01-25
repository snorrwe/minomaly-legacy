#pragma once
#include "collider.h"
#include "quadtree.h"
#include "transform.h"
#include "vector2.h"
#include <algorithm>
#include <memory>
#include <vector>

// TODO: work service
#include <future>
#include <thread>

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
    virtual void add(Collider*) = 0;
    virtual void remove(Collider*) = 0;
    virtual void setWorldBox(BoundingBox const& box) = 0;
};

class PhysicsSystem : public IPhysicsSystem
{
public:
    const size_t WORLD_CAPACITY = 64;

    static std::shared_ptr<PhysicsSystem> create() { return std::make_shared<PhysicsSystem>(); }

    virtual void update();
    virtual void add(Collider*);
    virtual void remove(Collider*);
    virtual void setWorldBox(BoundingBox const& box);

    virtual std::shared_ptr<World> getWorld() { return world; }

protected:
    std::shared_ptr<World> world =
        std::make_shared<World>(BoundingBox{{0, 0}, 5e8, 5e8}, nullptr, WORLD_CAPACITY);
    std::vector<Collider*> colliders;
};

} // namespace Mino
