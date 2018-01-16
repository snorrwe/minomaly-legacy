#pragma once
#include "bounding_box.h"
#include "component.h"
#include "observer.h"
#include "physics_component.h"
#include "physics_system.h"
#include "quadtree.h"
#include "scene.h"
#include "transform.h"
#include "vector2.h"
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

    virtual ~ColliderComponent();

    virtual void start();
    virtual void update();
    virtual void enable();
    virtual void disable();
    virtual void handleCollision(ColliderComponent const&);
    virtual Observable<ColliderComponent>& onCollision() { return *onCollisionSubject; }

    virtual void checkCollisions() = 0;
    virtual void addToWorld();
    virtual void removeFromWorld();

protected:
    void updateCornersByDeltaPos();

    Transform::TransformRef transform;
    Vector2<double> lastPos = {0, 0};
    Vector2<double> deltaPos = {0, 0};
    std::weak_ptr<IPhysicsSystem> physicsSystem;
    std::weak_ptr<World> world;
    std::vector<Vector2<double>> corners;

    std::shared_ptr<Subject<ColliderComponent>> onCollisionSubject =
        std::make_shared<Subject<ColliderComponent>>();
};

} // namespace Mino
