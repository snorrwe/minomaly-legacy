#pragma once
#include "mino.h"
#include <cmath>
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
        eggCollider->set(20, 30, {5.0f, 0.0f});
        eggCollider->onCollision().subscribe([&](auto const& cd) { handleCollision(cd); });
        eggCollider->setLayers(0x1);
        body->addCollider(eggCollider);
        height = eggCollider->getHeight();
        bottomCollider = eggCollider.get();
        /*
                bottomCollider = gameObject->addComponent<Mino::BoxColliderComponent>().get();
                bottomCollider->set(28, 2, {1, -1});
                bottomCollider->setLayers(0x2);
                */
    }

    void handleCollision(Mino::CollisionData const& cd)
    {
        auto boxA = cd.first.asBoundingBox();
        auto boxB = cd.second.asBoundingBox();

        auto diffY = boxB.getCenter().y() - boxA.getCenter().y();
        if (diffY + height < boxA.getHeight() * 0.5f + boxB.getHeight() * 0.5f)
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
        bool touchingGround = bottomCollider->touchingAny();
        if (state == State::Grounded)
        {
            if (!touchingGround && transform->position().y() > bottom)
            {
                state = State::Falling;
            }
            if (input->isDown(SDLK_UP))
            {
                velocity = {velocity.x(), 525.0f};
                state = State::Airborn;
                airTime = 0.0;
            }
        }
        else if (state == State::Airborn)
        {
            airTime += time->deltaTime();
            if (airTime > 0.3f)
            {
                velocity = {velocity.x(), 0.0f};
                state = State::Falling;
            }
        }
        else if (state == State::Falling)
        {
            velocity = {velocity.x(), velocity.y() - gravity};
            auto& pos = transform->position();
            if (pos.y() <= bottom)
            {
                state = State::Grounded;
                velocity = {velocity.x(), 0.0};
                pos = {pos.x(), float(bottom)};
            }
        }

        const float sv = 400.0f;
        float x = 0.0f;
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
    }

    std::shared_ptr<Mino::IInputSystem> input;
    int bottom;
    float height;

private:
    std::shared_ptr<Mino::PhysicsComponent> body;
    std::shared_ptr<Mino::ITimeService> time;
    Mino::Transform::TransformRef transform;
    Mino::BoxColliderComponent* bottomCollider;

    Mino::Vector2<float> velocity = {0, 0};
    const float gravity = 50.0;
    State state = State::Grounded;
    float airTime = 1000.0;
};
