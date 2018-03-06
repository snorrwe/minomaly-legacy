#include "physics_system.h"

using namespace Mino;

void PhysicsSystem::update()
{
    world->clear();
    for (auto& collider : colliders)
    {
        collider->updatePosition();
        collider->addToWorld();
    }
    auto handles = std::vector<std::future<std::vector<World::Node>>>{};
    for (auto& collider : colliders)
    {
        handles.push_back(workService->requestWork<std::vector<World::Node>>(
            [collider]() { return collider->checkCollisions(); }));
    }
    for (auto i = handles.rbegin(); i != handles.rend(); ++i)
    {
        i->wait();
    }
    auto handleIt = handles.begin();
    for (auto& collider : colliders)
    {
        collider->handleCollisions(std::move(handleIt->get()));
        ++handleIt;
    }
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
    world = std::make_unique<World>(box, WORLD_CAPACITY);
}
