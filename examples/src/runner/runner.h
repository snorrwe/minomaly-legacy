#pragma once
#include "camera_control.h"
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
    using MediaContainer = std::vector<std::shared_ptr<Mino::Asset>>;

    Mino::IInputSystem* input;
    Mino::ITimeService* time;
    MediaContainer images;

public:
    virtual void start();

private:
    void initPlatforms();
    void createPlatform(Mino::Vector2<float> const& position,
                        Mino::Vector2<float> const& scale = {1.f, 1.f});
};
