#include "physics_component.h"

using namespace Mino;

void PhysicsComponent::start()
{
    transform = gameObject->getTransform();
    time = Services::get<ITimeService>();
}

void PhysicsComponent::update()
{
    auto deltaTime = time->deltaTime();
    auto position = transform->getPosition();
    velocity = velocity + acceleration;
    auto x = position.x() + velocity.x() / deltaTime;
    auto y = position.y() + velocity.y() / deltaTime;
    transform->setPosition(x, y);
}

void PhysicsComponent::handleCollision(Vector2<double> const& point)
{
    auto deltaTime = time->deltaTime();
    auto position = transform->getPosition();
    auto x = position.x() - velocity.x() / deltaTime;
    auto y = position.y() - velocity.y() / deltaTime;
    auto originalPos = Vector2<double>(x, y);

    auto traveled = point - originalPos;

    Vector2<double> bounceVector{velocity.x(), velocity.y()};
    bounceVector.rotateDeg(180.0);
    bounceVector = bounceVector * (velocity.length() / traveled.length());

    auto result = point + bounceVector;

    transform->setPosition(result);
}
