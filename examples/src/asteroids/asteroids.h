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

class Program : public Mino::Application
{
    using RotationData = Mino::RenderData::RotationData;
    using Subscriptions = std::array<Mino::ISubscription, 1>;
    using Animations = std::vector<Mino::SpriteAnimationData::Animation>;
    using SpriteSheet = std::shared_ptr<Mino::SpriteSheet>;

    Animations animations = {};
    SpriteSheet asteroidSheet;

public:
    virtual void start();
};
