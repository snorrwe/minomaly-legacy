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
    using AudioContainer = std::vector<std::shared_ptr<Mino::Audio>>;
    using Animations = std::vector<Mino::SpriteAnimationData::Animation>;

    Mino::Rigidbody* body;
    Mino::AudioPlayerComponent* audioPlayer;
    Mino::BoxColliderComponent* bottomCollider;

    Animations animations = {};
    MediaContainer images = {};
    AudioContainer audio = {};

    Mino::ITimeService* time;

    Mino::Vector2<float> velocity = {0, 0};
    const float gravity = 30.0;
    State state = State::Grounded;
    float airTime = 1000.0;

public:
    Mino::IInputSystem* input;
    int bottom;
    float height;

    virtual void start()
    {
        time = Mino::Services::get<Mino::ITimeService>().get();
        body = gameObject->getComponent<Mino::Rigidbody>();
        auto eggCollider = gameObject->getComponent<Mino::BoxColliderComponent>();
        eggCollider->set(30, 30, {0.0f, 0.0f});
        eggCollider->setLayers(0x1);
        body->addCollider(*eggCollider);
        height = eggCollider->getHeight();

        bottomCollider = gameObject->addComponent<Mino::BoxColliderComponent>();
        bottomCollider->set(10, 12, {10.0f, -2.0f});
        bottomCollider->setLayers(0x2);

        auto assetSys = gameObject->minomaly()->getAssets();
        images = assetSys->loadSpriteSheet("assets/runner/egg.png", {
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

        audio.push_back(assetSys->loadWAV("assets/runner/Dodge01.wav"));
        audioPlayer = gameObject->getComponent<Mino::AudioPlayerComponent>();
    }

    virtual void update()
    {
        const bool touchingGround = bottomCollider->touchingAny();

        switch (state)
        {
        case State::Grounded:
            handleGrounded(touchingGround);
            break;
        case State::Airborn:
            handleAirborn(touchingGround);
            break;
        case State::Falling:
            handleFalling(touchingGround);
            break;
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

    State getState() { return state; }

private:
    void handleGrounded(const bool touchingGround)
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
            audioPlayer->play(*audio.at(0), 1);
        }
    }

    void handleAirborn(const bool touchingGround)
    {
        airTime += time->deltaTime();
        if (airTime > 0.2f)
        {
            velocity = {velocity.x(), 0.0f};
            state = State::Falling;
        }
    }

    void handleFalling(const bool touchingGround)
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
};
