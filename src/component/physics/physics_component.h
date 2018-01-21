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
        ColliderComponent* coll;
        ISubscription sub;
    };

    struct Material
    {
        float restitution = 1.0f;
    };

    virtual ~PhysicsComponent() {}

    virtual void start();
    virtual void update();

    void setVelocity(Vector2<float> const& v);
    Vector2<float> const& getVelocity() const { return velocity; }

    Material& getMaterial() { return material; }
    Material const& getMaterial() const { return material; }
    void setMaterial(Material const& value) { material = value; }

    void addCollider(ColliderComponent &coll);

private:
    void resolveCollision(CollisionData const& collistionData);

    Transform::TransformRef transform = nullptr;
    std::shared_ptr<ITimeService> time = nullptr;
    Vector2<float> normalDirection = {0, 0};
    Vector2<float> velocity = {0, 0};
    Material material = {};

    Vector2<float> lastPosition = {0, 0};

    std::vector<ColliderRef> subs = {};
};

} // namespace Mino
