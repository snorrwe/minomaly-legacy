#pragma once
#include "mino.h"
#include <memory>

namespace
{

using namespace Mino;

class AsteroidComponent : public Mino::Component
{
    using Animations = std::vector<Mino::SpriteAnimationData::Animation>;
    using SpriteSheet = std::shared_ptr<Mino::SpriteSheet>;

    Animations animations = {};
    SpriteSheet asteroidSheet;
    size_t worldWidth = 0;

public:
    void setup(size_t ww, Mino::Vector2<float> const& velocity)
    {
        worldWidth = ww;
        gameObject->getComponent<Rigidbody>()->setVelocity(velocity);
    }

    virtual void start()
    {
        asteroidSheet = gameObject->getApplication()->getEngineCore()->getAssets()->loadSpriteSheet(
            "assets/asteroids/animated_asteroid.png",
            {
                {0, 0, 70, 70},   {0, 70, 70, 70},   {70, 0, 70, 70},   {70, 70, 70, 70},
                {140, 0, 70, 70}, {140, 70, 70, 70}, {210, 0, 70, 70},  {210, 70, 70, 70},
                {280, 0, 70, 70}, {280, 70, 70, 70}, {350, 0, 70, 70},  {350, 70, 70, 70},
                {420, 0, 70, 70}, {420, 70, 70, 70}, {490, 0, 70, 70},  {490, 70, 70, 70},
                {560, 0, 70, 70}, {560, 70, 70, 70}, {630, 0, 70, 70},  {630, 70, 70, 70},
                {700, 0, 70, 70}, {700, 70, 70, 70}, {770, 0, 70, 70},  {770, 70, 70, 70},
                {840, 0, 70, 70}, {840, 70, 70, 70}, {910, 0, 70, 70},  {910, 70, 70, 70},
                {980, 0, 70, 70}, {980, 70, 70, 70}, {1050, 0, 70, 70}, {1050, 70, 70, 70},
            });
        auto asteroidFrames = std::vector<Mino::SpriteAnimationData::Frame>{};
        for (auto& image : *asteroidSheet)
        {
            asteroidFrames.push_back({{}, 1.0f / asteroidSheet->size(), image.get()});
        }
        animations = {
            Mino::SpriteAnimationData::Animation{
                asteroidFrames,
                Mino::SpriteAnimationData::Animation::Loop,
            },
        };
        gameObject->getComponent<SpriteRendererComponent>();
        gameObject->getComponent<SpriteAnimatorComponent>()->startAnimation(animations[0]);
        auto collider = gameObject->getComponent<BoxColliderComponent>();
        collider->set(50.f, 50.f, {20.f, 20.f});
        gameObject->getComponent<Rigidbody>()->addCollider(*collider);
    }
};
} // namespace
