#include "physics_system.h"

using namespace Mino;

void PhysicsSystem::update()
{
    for (auto& collider : colliders)
    {
        collider->updatePosition();
        collider->addToWorld();
    }
    auto handles = std::vector<std::future<std::vector<World::Node>>>{};
    for (auto& collider : colliders)
    {
        handles.push_back(
            std::async(std::launch::async, [collider]() { return collider->checkCollisions(); }));
    }
    for (auto& handle : handles)
    {
        handle.wait();
    }
    auto handleIt = handles.begin();
    for (auto& collider : colliders)
    {
        collider->handleCollisions(std::move((handleIt++)->get()));
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
    world = std::make_shared<World>(box, nullptr, WORLD_CAPACITY);
}
