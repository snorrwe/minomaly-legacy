#pragma once
#include "collider.h"
#include "quadtree.h"
#include "services.h"
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
    virtual void add(Collider*) = 0;
    virtual void remove(Collider*) = 0;
    virtual void setWorldBox(BoundingBox const& box) = 0;

    virtual World* getWorld() = 0;
};

class PhysicsSystem : public IPhysicsSystem
{
public:
    const size_t WORLD_CAPACITY = 64;

    static std::shared_ptr<PhysicsSystem> create()
    {
        auto workService = std::static_pointer_cast<WorkService>(Services::get<IWorkService>());
        return std::make_shared<PhysicsSystem>(workService);
    }

    PhysicsSystem(std::shared_ptr<WorkService> workService) : workService(std::move(workService)) {}
    virtual ~PhysicsSystem() {}

    virtual void update();
    virtual void add(Collider*);
    virtual void remove(Collider*);
    virtual void setWorldBox(BoundingBox const& box);

    virtual World* getWorld() { return world.get(); }

protected:
    std::unique_ptr<World> world
        = std::make_unique<World>(BoundingBox{{0, 0}, 5e8, 5e8}, WORLD_CAPACITY);
    std::vector<Collider*> colliders = {};
    std::shared_ptr<WorkService> workService = nullptr;
};

} // namespace Mino
