#include "physics_component.h"

using namespace Mino;

void PhysicsComponent::start()
{
    transform = gameObject->getTransform();
    time = Services::get<ITimeService>();
}

void PhysicsComponent::update()
{
    auto deltaTime = 1 / time->deltaTime();
    auto position = transform->getPosition();
    position = position + (velocity * deltaTime);
    transform->setPosition(position);
}

void PhysicsComponent::addCollider(std::shared_ptr<ColliderComponent> coll)
{
    subs.push_back(
        {coll, coll->onCollision().subscribe([&](auto const& cd) { resolveCollision(cd); })});
}

void PhysicsComponent::resolveCollision(CollisionData const& collistionData)
{
    auto box1 = collistionData.first.asBoundingBox();
    auto box2 = collistionData.second.asBoundingBox();

    auto idealDeltaW = (box1.getWidth() + box2.getWidth()) * 0.5;
    auto idealX = box1.getCenter().x() > box2.getCenter().x()
                      ? box1.getCenter().x() - box2.getCenter().x()
                      : box2.getCenter().x() - box1.getCenter().x();
    auto deltaX = idealDeltaW - idealX;

    auto idealDeltaH = (box1.getHeight() + box2.getHeight()) * 0.5;
    auto idealY = box1.getCenter().y() > box2.getCenter().y()
                      ? box1.getCenter().y() - box2.getCenter().y()
                      : box2.getCenter().y() - box1.getCenter().y();
    auto deltaY = idealDeltaH - idealY;

    auto corrected = transform->getPosition();
    if (deltaX < deltaY)
    {
        const auto c = box1.getCenter().x() > box2.getCenter().x() ? 1 : -1;
        auto x = c * deltaX;
        corrected = {corrected.x() + x, corrected.y()};
    }
    else if (deltaY < deltaX)
    {
        const auto c = box1.getCenter().y() > box2.getCenter().y() ? 1 : -1;
        auto y = c * deltaY;
        corrected = {corrected.x(), corrected.y() + y};
    }
    else
    {
        const auto cx = box1.getCenter().x() > box2.getCenter().x() ? 1 : -1;
        auto x = cx * deltaX;
        const auto cy = box1.getCenter().y() > box2.getCenter().y() ? 1 : -1;
        auto y = cy * deltaY;
        corrected = {corrected.x() + x, corrected.y() + y};
    }

    transform->setPosition(corrected);
    transform->flip();
    transform->reset();
}

void PhysicsComponent::setVelocity(Vector2<double> const& v)
{
    velocity = v;
    auto norm = v.normalized();
    normalDirection = {-norm.y(), norm.x()};
}
