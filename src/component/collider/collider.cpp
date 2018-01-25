#include "collider.h"

using namespace Mino;

ColliderComponent::~ColliderComponent()
{
    if (auto phs = physicsSystem.lock(); phs)
    {
        phs->remove(this);
    }
}

void ColliderComponent::start()
{
    transform = gameObject->getTransform();
    lastPos = transform->absolute().position;
    physicsSystem = gameObject->getScene()->getEngineCore()->getPhysicsSystem();
    world = physicsSystem.lock()->getWorld();
    addToWorld();
    physicsSystem.lock()->add(this);
}

void ColliderComponent::enable()
{
    addToWorld();
    physicsSystem.lock()->add(this);
    Component::enable();
}

void ColliderComponent::disable()
{
    physicsSystem.lock()->remove(this);
    Component::disable();
}

void ColliderComponent::handleCollision(ColliderComponent const& coll)
{
    touching.insert(&coll);
    onCollisionSubject->next({*this, coll});
}

void ColliderComponent::updatePosition()
{
    auto& currentPos = transform->absolute().position;
    deltaPos = currentPos - lastPos;
    updateCornersByPosition(currentPos);
    lastPos = currentPos;
}

void ColliderComponent::addToWorld()
{
    auto wrld = world.lock();
    wrld->insert({asBoundingBox().getCenter(), this});
}

Observable<CollisionData>& ColliderComponent::onCollision() { return *onCollisionSubject; }

Observable<CollisionData>& ColliderComponent::onCollisionResolve()
{
    return *onCollisionResolutionSubject;
}

std::vector<typename ColliderComponent::World::Node> ColliderComponent::checkCollisions() const
{
    auto result = std::vector<World::Node>{};
    result.reserve(5);
    auto box = asBoundingBox();
    world.lock()->queryRange(box, [&](const auto& i) {
        if (i.item != this && i.item->asBoundingBox().intersects(box)) result.push_back(i);
    });
    return result;
}

void ColliderComponent::handleCollisions(
    std::vector<typename ColliderComponent::World::Node> const& points)
{

    auto currentlyTouching = TouchContainer{};
    if (points.empty())
    {
        handleResolvedCollisions(currentlyTouching);
        touching.clear();
        return;
    }

    for (auto& i : points)
    {
        if (layers & i.item->layers)
        {
            i.item->handleCollision(*this);
            currentlyTouching.insert(i.item);
        }
    }

    handleResolvedCollisions(currentlyTouching);
}

void ColliderComponent::handleResolvedCollisions(TouchContainer& currentlyTouching)
{
    auto noLongerTouching = TouchContainer{};
    std::set_difference(touching.begin(), touching.end(), currentlyTouching.begin(),
                        currentlyTouching.end(),
                        std::inserter(noLongerTouching, noLongerTouching.begin()));
    for (auto& collider : noLongerTouching)
    {
        onCollisionResolutionSubject->next({*this, *collider});
    }
    touching = std::move(currentlyTouching);
}

void ColliderComponent::removeSelf(std::vector<World::Node>& points) const
{
    auto it =
        std::find_if(points.begin(), points.end(), [&](auto const& i) { return i.item == this; });
    while (it != points.end())
    {
        points.erase(it);
        it = std::find_if(points.begin(), points.end(),
                          [&](auto const& i) { return i.item == this; });
    }
}
