#include "collider.h"

using namespace Mino;

ColliderComponent::~ColliderComponent()
{
    for (auto i = corners.begin(); i != corners.end(); ++i)
    {
        world.lock()->erase({*i, this});
    }
}

void ColliderComponent::start()
{
    transform = gameObject->getTransform();
    lastPos = transform->getPosition();
    physicsSystem = gameObject->getScene()->getEngineCore()->getPhysicsSystem();
    world = physicsSystem.lock()->getWorld();
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
{
    if (deltaPos)
    {
        auto delta = coll.lastPos - lastPos;
        transform->setPosition(lastPos - (delta * 0.02));
        transform->flip();
        transform->reset();
    }
    onCollisionSubject->next(coll);
}

void ColliderComponent::update()
{
    auto currentPos = transform->getPosition();
    deltaPos = currentPos - lastPos;
    if (deltaPos)
    {
        updateCornersByDeltaPos();
    }
    lastPos = currentPos;
}

void ColliderComponent::updateCornersByDeltaPos()
{
    auto wrld = world.lock();
    for (auto i = corners.begin(); i != corners.end(); ++i)
    {
        IPhysicsSystem::World::Node from{*i, this};
        IPhysicsSystem::World::Node to{*i + deltaPos, this};
        wrld->move(from, to);
        *i = to.pos;
    }
}

void ColliderComponent::addToWorld()
{
    auto wrld = world.lock();
    for (auto i = corners.begin(); i != corners.end(); ++i)
    {
        wrld->insert({*i, this});
    }
}

void ColliderComponent::removeFromWorld()
{
    auto wrld = world.lock();
    for (auto i = corners.begin(); i != corners.end(); ++i)
    {
        wrld->erase({*i, this});
    }
}