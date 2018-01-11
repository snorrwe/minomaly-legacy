#pragma once
#include "component.h"
#include "services.h"
#include "time_service.h"
#include "transform.h"
#include "vector2.h"

namespace Mino
{

class PhysicsComponent : public Component
{
public:
    virtual ~PhysicsComponent() {}

    virtual void start();
    virtual void update();

    virtual void handleCollision(Vector2<double> const& point);

    void setVelocity(Vector2<double> const& v) { velocity = v; }
    Vector2<double> const& getVelocity() { return velocity; }

private:
    Transform* transform = nullptr;
    ITimeService* time = nullptr;
    Vector2<double> velocity = Vector2<double>{0, 0};
};

} // namespace Mino
