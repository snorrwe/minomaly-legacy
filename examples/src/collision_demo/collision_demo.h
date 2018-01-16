#pragma once
#include "mino.h"
#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

const size_t SCREEN_WIDTH = 640;
const size_t SCREEN_HEIGHT = 480;

class Program : public Mino::Scene
{
public:
    using RotationData = Mino::RenderData::RotationData;

    typedef std::vector<Mino::ISubscription> Subscriptions;
    typedef std::vector<std::shared_ptr<Mino::Texture>> MediaContainer;

    Program(std::shared_ptr<Mino::EngineCore> core);
    Program(Program const&) = delete;
    Program(Program&&) = delete;
    ~Program();

    Program& operator=(Program const&) = delete;
    Program& operator=(Program&&) = delete;

    virtual void update();
    virtual void start();

private:
    Subscriptions subs;
    std::vector<std::shared_ptr<Mino::GameObject>> gameObjects;
    std::shared_ptr<Mino::PhysicsComponent> eggPhysics;
    std::shared_ptr<Mino::IInputSystem> input;
    MediaContainer images;
};
