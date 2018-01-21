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
        eggCollider->set(30, 30, {0.0f, 0.0f});
        eggCollider->setLayers(0x1);
        body->addCollider(*eggCollider);
        height = eggCollider->getHeight();

        bottomCollider = gameObject->addComponent<Mino::BoxColliderComponent>();
        bottomCollider->set(28, 32, {1.0f, -2.0f});
        bottomCollider->setLayers(0x2);
    }

    virtual void update()
    {
        // reportState();

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
            auto& pos = transform->position();
            if (touchingGround || pos.y() <= bottom)
            {
                state = State::Grounded;
                velocity = {velocity.x(), 0.0};
                if (pos.y() <= bottom)
                {
                    pos = {pos.x(), float(bottom)};
                }
            }
            else
            {
                velocity = {velocity.x(), velocity.y() - gravity};
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
    void reportState()
    {
        switch (state)
        {
        case State::Grounded:
            std::cout << "Grounded\n";
            break;
        case State::Airborn:
            std::cout << "Airborn\n";
            break;
        case State::Falling:
            std::cout << "Falling\n";
            break;
        }
    }

    Mino::PhysicsComponent* body;
    std::shared_ptr<Mino::ITimeService> time;
    Mino::Transform::TransformRef transform;
    Mino::BoxColliderComponent* bottomCollider;

    Mino::Vector2<float> velocity = {0, 0};
    const float gravity = 50.0;
    State state = State::Grounded;
    float airTime = 1000.0;
};
