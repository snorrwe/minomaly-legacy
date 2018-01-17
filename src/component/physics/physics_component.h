#pragma once
#include "collider.h"
#include "component.h"
#include "observer.h"
#include "services.h"
#include "time_service.h"
#include "transform.h"
#include "vector2.h"
#include <cmath>
#include <memory>
#include <vector>

namespace Mino
{

class ColliderComponent;
struct CollisionData;

class PhysicsComponent : public Component
{
public:
    struct ColliderRef
    {
        std::weak_ptr<ColliderComponent> coll;
        ISubscription sub;
    };

    virtual ~PhysicsComponent() {}

    virtual void start();
    virtual void update();

    void setVelocity(Vector2<double> const& v);
    Vector2<double> const& getVelocity() const { return velocity; }

    void addCollider(std::shared_ptr<ColliderComponent> coll);

private:
    void resolveCollision(CollisionData const& collistionData);

    Transform::TransformRef transform = nullptr;
    std::shared_ptr<ITimeService> time = nullptr;
    Vector2<double> normalDirection = {0, 0};
    Vector2<double> velocity = {0, 0};

    Vector2<double> lastPosition = {0, 0};

    std::vector<ColliderRef> subs = {};
};

} // namespace Mino
