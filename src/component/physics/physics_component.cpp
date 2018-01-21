#include "physics_component.h"

using namespace Mino;

void PhysicsComponent::start()
{
    transform = gameObject->getTransform();
    time = Services::get<ITimeService>();
}

void PhysicsComponent::update()
{
    auto deltaTime = time->deltaTime();
    auto position = transform->getPosition();
    position = position + velocity * deltaTime;
    transform->setPosition(position);
    lastPosition = position;
}

void PhysicsComponent::addCollider(ColliderComponent &coll)
{
    subs.push_back(
        {&coll, coll.onCollision().subscribe([&](auto const& cd) { resolveCollision(cd); })});
}

void PhysicsComponent::resolveCollision(CollisionData const& collistionData)
{
    auto box1 = collistionData.first.asBoundingBox();
    auto box2 = collistionData.second.asBoundingBox();

    auto idealDeltaW = (box1.getWidth() + box2.getWidth()) * 0.5f;
    auto idealX = box1.getCenter().x() > box2.getCenter().x()
                      ? box1.getCenter().x() - box2.getCenter().x()
                      : box2.getCenter().x() - box1.getCenter().x();
    auto deltaX = idealDeltaW - idealX;

    auto idealDeltaH = (box1.getHeight() + box2.getHeight()) * 0.5f;
    auto idealY = box1.getCenter().y() > box2.getCenter().y()
                      ? box1.getCenter().y() - box2.getCenter().y()
                      : box2.getCenter().y() - box1.getCenter().y();
    auto deltaY = idealDeltaH - idealY;

    const auto cx =
        box1.getCenter().x() > box2.getCenter().x() ? material.restitution : -material.restitution;
    const auto cy =
        box1.getCenter().y() > box2.getCenter().y() ? material.restitution : -material.restitution;
    auto x = cx * deltaX;
    auto y = cy * deltaY;

    if (deltaX >= deltaY)
        x = 0.0f;
    else if (deltaY >= deltaX)
        y = 0.0f;

    transform->setPosition(transform->getPosition() + Vector2<float>{x, y});
}

void PhysicsComponent::setVelocity(Vector2<float> const& v)
{
    velocity = v;
    auto norm = v.normalized();
    normalDirection = {-norm.y(), norm.x()};
}
