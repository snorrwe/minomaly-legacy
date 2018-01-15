#include "collider.h"

using namespace Mino;

void ColliderComponent::start()
{
    transform = gameObject->getTransform();
    physicsSystem = gameObject->getScene()->getEngineCore()->getPhysicsSystem();
    world = physicsSystem.lock()->getWorld();
    physicsComponent = gameObject->getComponent<PhysicsComponent>();
    if (!physicsComponent.lock())
    {
        physicsComponent = gameObject->addComponent<PhysicsComponent>();
    }
    addToWorld();
    physicsSystem.lock()->add(std::static_pointer_cast<ColliderComponent>(self.lock()));
}

void ColliderComponent::enable()
{
    addToWorld();
    physicsSystem.lock()->add(std::static_pointer_cast<ColliderComponent>(self.lock()));
    Component::enable();
}

void ColliderComponent::disable()
{
    removeFromWorld();
    physicsSystem.lock()->remove(std::static_pointer_cast<ColliderComponent>(self.lock()));
    Component::disable();
}

void ColliderComponent::handleCollision(ColliderComponent const& coll)
{ /* TODO */
}

void ColliderComponent::update()
{
    for (auto i = corners.begin(); i != corners.end(); ++i)
    {
        IPhysicsSystem::World::Node from{*i, this};
        IPhysicsSystem::World::Node to{*i + physicsComponent.lock()->getVelocity(), this};
        world.lock()->move(from, to);
        *i = to.pos;
    }
}

void ColliderComponent::addToWorld()
{
    for (auto i = corners.begin(); i != corners.end(); ++i)
    {
        world.lock()->insert({*i, this});
    }
}

void ColliderComponent::removeFromWorld()
{
    for (auto i = corners.begin(); i != corners.end(); ++i)
    {
        world.lock()->erase({*i, this});
    }
}