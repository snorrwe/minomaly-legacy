#pragma once
#include "egg.h"
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
    using MediaContainer = std::vector<std::shared_ptr<Mino::Asset>>;

    Mino::IInputSystem* input;
    Mino::ITimeService* time;
    Subscriptions subs;
    MediaContainer images;

public:
    virtual ~Program();

    virtual void update();
    virtual void start();
};
