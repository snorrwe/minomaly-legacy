#include "physics_system.h"

using namespace Mino;

void PhysicsSystem::update()
{
    for (auto& i : colliders)
    {
        i->updatePosition();
        i->addToWorld();
    }
    for (auto& i : colliders)
    {
        i->checkCollisions();
    }
    world->clear();
}

void PhysicsSystem::add(Collider* coll)
{
    if (std::find(colliders.begin(), colliders.end(), coll) == colliders.end())
    {
        colliders.push_back(coll);
    }
}

void PhysicsSystem::remove(Collider* coll)
{
    auto it = std::find(colliders.begin(), colliders.end(), coll);
    if (it != colliders.end())
    {
        colliders.erase(it);
    }
}

void PhysicsSystem::setWorldBox(BoundingBox const& box)
{
    world = std::make_shared<World>(box, nullptr, 128);
}
