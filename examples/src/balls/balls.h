#pragma once
#include "ball.h"
#include "mino.h"
#include <array>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <vector>

const size_t SCREEN_WIDTH = 1366;
const size_t SCREEN_HEIGHT = 768;

class Program : public Mino::Application
{
public:
    using RotationData = Mino::RenderData::RotationData;
    using Subscriptions = std::array<Mino::ISubscription, 1>;
    using MediaContainer = std::vector<std::shared_ptr<Mino::Texture>>;

    ~Program();

    virtual void update();
    virtual void start();

private:
    std::shared_ptr<Mino::IInputSystem> input;
    std::shared_ptr<Mino::IRenderSystem> renderer;
    std::vector<Mino::GameObject*> balls = {};
    Subscriptions subs;
    MediaContainer images;
};
