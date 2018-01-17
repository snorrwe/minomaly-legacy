#pragma once
#include "mino.h"
#include <memory>

enum class State
{
    Grounded,
    Airborn,
    Falling
};

class EggComponent : public Mino::Component
{
public:
    virtual void start()
    {
        body = gameObject->getComponent<Mino::PhysicsComponent>();
        time = Mino::Services::get<Mino::ITimeService>();
        transform = gameObject->getTransform();
        auto eggCollider = gameObject->getComponent<Mino::BoxColliderComponent>();
        eggCollider->set(30, 30);
        eggCollider->onCollision().subscribe([&](auto const& cd) { handleCollision(cd); });
        eggCollider->setLayers(0x1);
        body->addCollider(eggCollider);
        height = eggCollider->getHeight();

        auto bottomCollider = gameObject->addComponent<Mino::BoxColliderComponent>();
        bottomCollider->set(2, 31);
        bottomCollider->onCollision().subscribe([&](auto const& cd) { handleGroundCollision(cd); });
        bottomCollider->setLayers(0x2);
    }

    void handleGroundCollision(Mino::CollisionData const& cd) { touchingGround = true; }

    void handleCollision(Mino::CollisionData const& cd)
    {
        auto boxA = cd.first.asBoundingBox();
        auto boxB = cd.second.asBoundingBox();

        auto diffY = boxB.getCenter().y() - boxA.getCenter().y();
        if (diffY + height >= boxA.getHeight() * 0.5 + boxB.getHeight() * 0.5)
        {
            if (state == State::Falling)
            {
                state = State::Grounded;
                velocity = {velocity.x(), 0.0};
            }
        }
    }

    virtual void update()
    {
        const double sv = 400;
        if (state == State::Grounded)
        {
            if (!touchingGround && transform->getPosition().y() <= bottom)
            {
                state = State::Airborn;
            }
            if (input->isDown(SDLK_UP))
            {
                velocity = {velocity.x(), -525};
                state = State::Airborn;
                airTime = 0.0;
            }
        }
        else if (state == State::Airborn)
        {
            airTime += time->deltaTime();
            if (airTime > 0.3)
            {
                velocity = {velocity.x(), 0.0};
                state = State::Falling;
            }
        }
        else if (state == State::Falling)
        {
            velocity = {velocity.x(), velocity.y() + gravity};
            const auto pos = transform->getPosition();
            if (pos.y() >= bottom) // TODO: replace this with colliders
            {
                state = State::Grounded;
                velocity = {velocity.x(), 0.0};
                transform->setPosition({pos.x(), (double)bottom});
            }
        }

        double x = 0.0;
        if (input->isDown(SDLK_LEFT))
        {
            x -= sv;
        }
        if (input->isDown(SDLK_RIGHT))
        {
            x += sv;
        }
        velocity = {x, velocity.y()};
        body->setVelocity(velocity);
        touchingGround = false;
    }

    std::shared_ptr<Mino::IInputSystem> input;
    int bottom;
    double height;
    bool touchingGround = false;

private:
    std::shared_ptr<Mino::PhysicsComponent> body;
    std::shared_ptr<Mino::ITimeService> time;
    Mino::Transform::TransformRef transform;

    Mino::Vector2<double> velocity = {0, 0};
    const double gravity = 10.0;
    State state = State::Grounded;
    double airTime = 1000.0;
};
