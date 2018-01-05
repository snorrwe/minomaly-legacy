#pragma once
#include "camera.h"
#include "color.h"
#include "core.h"
#include "input.h"
#include "renderdata.h"
#include "surface.h"
#include "window.h"
#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

const size_t SCREEN_WIDTH = 640;
const size_t SCREEN_HEIGHT = 480;

class Program : public Mino::ILogicSystem
{
public:
    using RotationData = Mino::RenderData::RotationData;

    typedef std::array<Mino::ISubscription, 1> Subscriptions;
    typedef std::vector<std::shared_ptr<Mino::Texture>> MediaContainer;

    Program(std::shared_ptr<Mino::Core> core);
    Program(Program const&) = delete;
    Program(Program&&) = delete;
    ~Program();

    Program& operator=(Program const&) = delete;
    Program& operator=(Program&&) = delete;

    virtual void update();
    virtual void start();

private:
    std::shared_ptr<Mino::Texture> loadTexture(std::string const& name, bool flag = false,
                                               Mino::Color const* color = nullptr);
    SDL_Rect getRect(int x, int y, int w, int h);

    uint8_t count = 0;
    std::shared_ptr<Mino::IInputSystem> input;
    std::shared_ptr<Mino::IRenderSystem> renderer;
    Subscriptions subs;
    MediaContainer images;
};
