#include "physics_system.h"

using namespace Mino;

void PhysicsSystem::update()
{
    for (auto i = colliders.begin(); i != colliders.end(); ++i)
    {
        i->lock()->checkCollisions();
    }
}

void PhysicsSystem::add(std::weak_ptr<Collider> const& coll)
{
    if (std::find_if(colliders.begin(), colliders.end(),
                     [&](auto const& c) { return c.lock() == coll.lock(); }) == colliders.end())
    {
        colliders.push_back(coll);
    }
}

void PhysicsSystem::remove(std::weak_ptr<Collider> const& coll)
{
    auto it = std::find_if(colliders.begin(), colliders.end(),
                           [&](auto const& c) { return c.lock() == coll.lock(); });
    if (it != colliders.end())
    {
        colliders.erase(it);
    }
}
