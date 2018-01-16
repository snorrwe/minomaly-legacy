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
    auto x = position.x() + (velocity.x() / deltaTime);
    auto y = position.y() + (velocity.y() / deltaTime);
    transform->setPosition(x, y);
}
