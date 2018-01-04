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

class Program : public ILogic
{
public:
    using RotationData = RenderData::RotationData;

    typedef std::array<ISubscription, 1> Subscriptions;
    typedef std::vector<std::shared_ptr<Texture>> MediaContainer;

    Program(std::shared_ptr<Core> core);
    Program(Program const&) = delete;
    Program(Program&&) = delete;
    ~Program();

    Program& operator=(Program const&) = delete;
    Program& operator=(Program&&) = delete;

    virtual void update();
    virtual void start();

private:
    std::shared_ptr<Texture> loadTexture(std::string const& name, bool flag = false,
                                         Color const* color = nullptr);
    SDL_Rect getRect(int x, int y, int w, int h);

    uint8_t count = 0;
    std::shared_ptr<Input> input;
    std::shared_ptr<Renderer> renderer;
    Subscriptions subs;
    MediaContainer images;
};
