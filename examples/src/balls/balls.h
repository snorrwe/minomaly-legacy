#pragma once
#include "ball.h"
#include "mino.h"
#include <array>
#include <cstdlib>
#include <ctime>
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

    typedef std::array<Mino::ISubscription, 1> Subscriptions;
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
    std::shared_ptr<Mino::IInputSystem> input;
    std::shared_ptr<Mino::IRenderSystem> renderer;
    std::vector<Mino::GameObject*> balls = {};
    Subscriptions subs;
    MediaContainer images;
};
