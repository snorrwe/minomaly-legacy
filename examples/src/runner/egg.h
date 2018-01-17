#pragma once
#include "mino.h"
#include <memory>

class EggComponent : public Mino::Component
{
public:
    virtual void start() { body = gameObject->getComponent<Mino::PhysicsComponent>(); }

    virtual void update()
    {
        Mino::Vector2<double> velocity = {0, 0};
        double sv = 200;
        if (input->isDown(SDLK_LEFT))
        {
            velocity = {velocity.x() - sv, velocity.y()};
        }
        if (input->isDown(SDLK_RIGHT))
        {
            velocity = {velocity.x() + sv, velocity.y()};
        }
        body->setVelocity(velocity);
    }

    std::shared_ptr<Mino::IInputSystem> input;

private:
    std::shared_ptr<Mino::PhysicsComponent> body;
};
