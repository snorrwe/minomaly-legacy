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
    using MediaContainer = std::shared_ptr<Mino::SpriteSheet>;
    using Animations = std::vector<Mino::SpriteAnimationData::Animation>;

    Mino::Rigidbody* body;
    Mino::ITimeService* time;
    Mino::Transform::TransformRef transform;
    Mino::BoxColliderComponent* bottomCollider;
    Animations animations = {};
    MediaContainer images = {};

    Mino::Vector2<float> velocity = {0, 0};
    const float gravity = 50.0;
    State state = State::Grounded;
    float airTime = 1000.0;
    bool shrink = true;

public:
    Mino::IInputSystem* input;
    int bottom;
    float height;

    virtual void start()
    {
        time = Mino::Services::get<Mino::ITimeService>().get();
        transform = gameObject->getTransform();
        body = gameObject->getComponent<Mino::Rigidbody>();
        auto eggCollider = gameObject->getComponent<Mino::BoxColliderComponent>();
        eggCollider->set(30, 30, {0.0f, 0.0f});
        eggCollider->setLayers(0x1);
        body->addCollider(*eggCollider);
        height = eggCollider->getHeight();

        bottomCollider = gameObject->addComponent<Mino::BoxColliderComponent>();
        bottomCollider->set(28, 32, {1.0f, -2.0f});
        bottomCollider->setLayers(0x2);

        images = gameObject->getApplication()->getEngineCore()->getAssets()->loadSpriteSheet(
            "assets/runner/egg.png", {
                                         {0, 0, 30, 30},
                                         {30, 0, 30, 30},
                                     });
        animations = {
            Mino::SpriteAnimationData::Animation{
                {
                    /** Frames:
                     *   (1) animation transformations (use default),
                     *   (2) duration in seconds,
                     *   (3) texture
                     */
                    /*(1)  (2)      (3)*/
                    {{}, 0.5f, images->at(0).get()}, // Frame 0
                    {{}, 0.2f, images->at(1).get()}, // Frame 1
                },
                Mino::SpriteAnimationData::Animation::Loop, // Flags: loop this animation
            },
        };

        gameObject->getComponent<Mino::SpriteAnimatorComponent>()->startAnimation(animations[0]);
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

        /*
        transform->rotation().angle += time->deltaTime();
        const auto PI = 3.1415f;
        if (transform->rotation().angle > 2 * PI) transform->rotation().angle = 0.0f;

        auto& scale = transform->scale();

        if (shrink)
        {
            if (scale.x() > 0.5f)
                scale = scale - Mino::Vector2<float>{time->deltaTime(), time->deltaTime()};
            else
                shrink = false;
        }
        else
        {
            if (scale.x() < 2.0f)
                scale = scale + Mino::Vector2<float>{time->deltaTime(), time->deltaTime()};
            else
                shrink = true;
        }*/
    }

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
};
