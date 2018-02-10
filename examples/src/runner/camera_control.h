#pragma once
#include "egg.h"
#include "mino.h"
#include <cassert>
#include <iostream>

namespace
{
using namespace Mino;

class CameraControllerComponent : public Mino::Component
{
    EggComponent* player;
    Transform::TransformRef camera;
    Mino::ITimeService* time;
    float weight = 0.1f;
    constexpr static float baseWeight = 6.f;

public:
    void setPlayer(EggComponent* value) { player = value; }
    void setCamera(Transform::TransformRef const& value) { camera = value; }

    virtual void start() override
    {
        time = Services::get<ITimeService>().get();
        assert(time);
    }

    virtual void update() override
    {
        switch (player->getState())
        {
        case State::Grounded:
            weight = baseWeight;
            break;
        case State::Airborn:
            weight = baseWeight * 0.25f;
            break;
        case State::Falling:
            weight = baseWeight * 0.5f;
            break;
        }

        auto target = player->getTransform()->getPosition();
        auto distance = target - transform->position();
        auto delta = distance * weight * time->deltaTime();
        camera->position() = camera->position() + delta;
        transform->position() = transform->position() + delta;
    }
};
} // namespace
