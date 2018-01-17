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
class ColliderComponent;

struct CollisionData
{
    ColliderComponent const& first;
    ColliderComponent const& second;
};

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
    virtual Observable<CollisionData>& onCollision() { return *onCollisionSubject; }

    virtual BoundingBox asBoundingBox() const = 0;
    virtual void checkCollisions() = 0;
    virtual void addToWorld();
    virtual void removeFromWorld();

    void setLayers(uint32_t l) { layers = l; }
    uint32_t getLayers() { return layers; }

    Vector2<double> getPositionDelta() const { return deltaPos; }

protected:
    void updateCornersByDeltaPos();

    uint32_t layers = 0x1;
    Transform::TransformRef transform;
    Vector2<double> lastPos = {0, 0};
    Vector2<double> deltaPos = {0, 0};
    std::weak_ptr<IPhysicsSystem> physicsSystem;
    std::weak_ptr<World> world;
    std::vector<Vector2<double>> corners;

    std::shared_ptr<Subject<CollisionData>> onCollisionSubject =
        std::make_shared<Subject<CollisionData>>();
};

} // namespace Mino
