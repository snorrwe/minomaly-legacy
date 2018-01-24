#pragma once
#include "mino.h"
#include <cstdlib>

class BallComponent : public Mino::Component
{
public:
    virtual void start()
    {
        auto eggCollider = gameObject->getComponent<Mino::BoxColliderComponent>();
        eggCollider->set(30, 30, {0.0f, 0.0f});
        eggCollider->onCollision().subscribe([&](auto const& collisionData) {
            auto vx = std::rand() % 100 - 50;
            auto vy = std::rand() % 100 - 50;
            body->setVelocity(Mino::Vector2<float>{vx, vy}.normalized() * 150.0f);
        });

        body = gameObject->getComponent<Mino::PhysicsComponent>();
        body->addCollider(*eggCollider);
        body->setMaterial({2.0f});

        auto vx = std::rand() % 100 - 50;
        auto vy = std::rand() % 100 - 50;
        body->setVelocity(Mino::Vector2<float>{vx, vy}.normalized() * 150.0f);
    }

private:
    Mino::PhysicsComponent* body;
};
