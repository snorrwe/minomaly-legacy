#include "physics_component.h"

using namespace Mino;

void PhysicsComponent::start()
{
    transform = gameObject->getTransform().get();
    time = Services::get<ITimeService>().get();
}

void PhysicsComponent::update()
{
    auto x = transform->getPosition().x() + velocity.x() * time->deltaTime();
    auto y = transform->getPosition().y() + velocity.y() * time->deltaTime();
    transform->setPosition(x, y);
}
