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
    if (deltaPos)
    {
        updateCornersByDeltaPos();
    }
    lastPos = currentPos;
}

void ColliderComponent::updateCornersByDeltaPos()
{
    for (auto& i : corners)
    {
        i = i + deltaPos;
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

Observable<CollisionData>& ColliderComponent::onCollision() { return *onCollisionSubject; }

Observable<CollisionData>& ColliderComponent::onCollisionResolve()
{
    return *onCollisionResolutionSubject;
}

void ColliderComponent::checkCollisions()
{
    auto box = asBoundingBox();
    auto points = world.lock()->queryRange(std::move(box));
    auto currentlyTouching = TouchContainer{};
    if (points.empty())
    {
        handleResolvedCollisions(currentlyTouching);
        touching.clear();
        return;
    }

    std::sort(points.begin(), points.end(),
              [](auto const& lhs, auto const& rhs) { return lhs.item < rhs.item; });
    points.erase(std::unique(points.begin(), points.end(),
                             [](auto const& lhs, auto const& rhs) { return lhs.item == rhs.item; }),
                 points.end());
    removeSelf(points);

    for (auto& i : points)
    {
        if (layers & i.item->layers)
        {
            i.item->handleCollision(*this);
            currentlyTouching.insert(i.item);
        }
    }

    handleResolvedCollisions(currentlyTouching);
    touching = std::move(currentlyTouching);
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
}

void ColliderComponent::removeSelf(std::vector<World::Node>& points)
{
    auto it =
        std::find_if(points.begin(), points.end(), [&](auto const& i) { return i.item == this; });
    if (it != points.end()) points.erase(it);
}
