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
        {coll, coll->onCollision().subscribe([&](auto const& collistionData) {
             auto box1 = collistionData.first.asBoundingBox();
             auto box2 = collistionData.second.asBoundingBox();

             auto correction = box2.getCenter() - box1.getCenter();
             correction = correction * -0.25;

             while (box1.intersects(box2))
             {
                 correction = correction * -2.0;
                 box1 = {box1.getCenter() + correction, box1.getWidth(), box2.getHeight()};
             }

             transform->setPosition(transform->getPosition() + correction);
             transform->flip();
             transform->reset();
         })});
}

void PhysicsComponent::setVelocity(Vector2<double> const& v)
{
    velocity = v;
    auto norm = v.normalized();
    normalDirection = {-norm.y(), norm.x()};
}
