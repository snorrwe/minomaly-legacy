#pragma once
#include "component.h"
#include "services.h"
#include "time_service.h"
#include "transform.h"
#include "vector2.h"
#include <memory>

namespace Mino
{

class PhysicsComponent : public Component
{
public:
    virtual ~PhysicsComponent() {}

    virtual void start();
    virtual void update();

    void setVelocity(Vector2<double> const& v) { velocity = v; }
    Vector2<double> const& getVelocity() const { return velocity; }

    void setAcceleration(Vector2<double> const& v) { acceleration = v; }
    Vector2<double> const& getAcceleration() const { return acceleration; }

private:
    Transform::TransformRef transform = nullptr;
    std::shared_ptr<ITimeService> time = nullptr;
    Vector2<double> velocity = Vector2<double>{0, 0};
    Vector2<double> acceleration = Vector2<double>{0, 0};
};

} // namespace Mino
