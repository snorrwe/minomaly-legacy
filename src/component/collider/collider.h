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
#include <algorithm>
#include <memory>
#include <set>
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
    using TouchContainer = std::set<ColliderComponent const*>;
    using CollisionEvent = std::unique_ptr<Subject<CollisionData>>;

    virtual ~ColliderComponent();

    virtual void start();
    virtual void enable();
    virtual void disable();
    virtual void updatePosition();
    virtual void handleCollision(ColliderComponent const&);
    virtual Observable<CollisionData>& onCollision();
    virtual Observable<CollisionData>& onCollisionResolve();

    virtual BoundingBox asBoundingBox() const = 0;

    virtual void checkCollisions();
    virtual void addToWorld();
    virtual void removeFromWorld();

    void setLayers(uint32_t l) { layers = l; }
    uint32_t getLayers() { return layers; }

    Vector2<float> getPositionDelta() const { return deltaPos; }

protected:
    void updateCornersByDeltaPos();

    uint32_t layers = 0x1;
    Transform::TransformRef transform;
    Vector2<float> lastPos = {0, 0};
    Vector2<float> deltaPos = {0, 0};
    std::weak_ptr<IPhysicsSystem> physicsSystem;
    std::weak_ptr<World> world;
    std::vector<Vector2<float>> corners = {};

private:
    void removeSelf(std::vector<World::Node>&);

    TouchContainer touching = {};
    CollisionEvent onCollisionSubject = std::make_unique<Subject<CollisionData>>();
    CollisionEvent onCollisionResolutionSubject = std::make_unique<Subject<CollisionData>>();
};

} // namespace Mino
