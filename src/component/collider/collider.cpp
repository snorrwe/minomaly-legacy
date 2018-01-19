#include "collider.h"

using namespace Mino;

ColliderComponent::~ColliderComponent()
{
    for (auto i = corners.begin(); i != corners.end(); ++i)
    {
        if (auto& w = world.lock(); w)
        {
            w->erase({*i, this});
        }
    }
}

void ColliderComponent::start()
{
    transform = gameObject->getTransform();
    lastPos = transform->absolute().position;
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
    if (layers & coll.layers) onCollisionSubject->next({*this, coll});
}

void ColliderComponent::update()
{
    auto currentPos = transform->absolute().position;
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
    if (!wrld)
    {
        disable();
        return;
    }
    for (auto& i : corners)
    {
        auto from = IPhysicsSystem::World::Node{i, this};
        auto to = IPhysicsSystem::World::Node{i + deltaPos, this};
        wrld->move(from, to);
        i = to.pos;
    }
}

void ColliderComponent::addToWorld()
{
    auto wrld = world.lock();
    for (auto& i : corners)
    {
        wrld->insert({i, this});
    }
}

void ColliderComponent::removeFromWorld()
{
    auto wrld = world.lock();
    for (auto& i : corners)
    {
        wrld->erase({i, this});
    }
}