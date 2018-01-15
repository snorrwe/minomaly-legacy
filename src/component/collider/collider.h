#pragma once
#include "bounding_box.h"
#include "component.h"
#include "observer.h"
#include "physics_component.h"
#include "physics_system.h"
#include "quadtree.h"
#include "scene.h"
#include "transform.h"
#include <memory>
#include <vector>

namespace Mino
{

class PhysicsComponent;
class IPhysicsSystem;

class ColliderComponent : public Component
{
public:
    using World = Quadtree<ColliderComponent>;

    virtual ~ColliderComponent() {}

    virtual void start();
    virtual void update();
    virtual void enable();
    virtual void disable();
    virtual void handleCollision(ColliderComponent const&);
    virtual Observable<ColliderComponent>& onCollision() { return onCollisionSubject; }

    virtual void checkCollisions() = 0;
    virtual void addToWorld();
    virtual void removeFromWorld();

protected:
    Transform::TransformRef transform;
    std::weak_ptr<PhysicsComponent> physicsComponent;
    std::weak_ptr<IPhysicsSystem> physicsSystem;
    std::weak_ptr<World> world;
    std::vector<Vector2<double>> corners;

    Subject<ColliderComponent> onCollisionSubject = Subject<ColliderComponent>();
};

} // namespace Mino
