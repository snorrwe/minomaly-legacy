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
            weight = 5.f;
            break;
        case State::Airborn:
            weight = 1.75f;
            break;
        case State::Falling:
            weight = 2.75f;
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
