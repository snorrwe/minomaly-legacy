#pragma once
#include "application.h"
#include "bounding_box.h"
#include "component.h"
#include "observer.h"
#include "physics_system.h"
#include "quadtree.h"
#include "rigidbody.h"
#include "transform.h"
#include "vector2.h"
#include <algorithm>
#include <memory>
#include <set>
#include <vector>

namespace Mino
{
class Rigidbody;
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

    std::vector<World::Node> checkCollisions(World const& world) const;
    void handleCollisions(std::vector<World::Node> const& points);

    void setLayers(uint32_t l) { layers = l; }
    uint32_t getLayers() const { return layers; }

    Vector2<float> getPositionDelta() const { return deltaPos; }
    TouchContainer const& getTouchingColliders() const { return touching; }
    bool touchingAny() const { return !touching.empty(); }

protected:
    virtual void updateCornersByPosition(Vector2<float> const&) = 0;

    uint32_t layers = 0x1;
    Vector2<float> lastPos = {0, 0};
    Vector2<float> deltaPos = {0, 0};
    IPhysicsSystem* physicsSystem;
    std::vector<Vector2<float>> corners = {};

private:
    void removeSelf(std::vector<World::Node>&) const;
    void handleResolvedCollisions(TouchContainer& currentlyTouching);

    TouchContainer touching = {};
    CollisionEvent onCollisionSubject = std::make_unique<Subject<CollisionData>>();
    CollisionEvent onCollisionResolutionSubject = std::make_unique<Subject<CollisionData>>();
};

} // namespace Mino
