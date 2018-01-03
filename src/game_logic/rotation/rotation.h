#pragma once
#include "SDL.h"
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

namespace Rotation
{

using RenderData::RotationData;

const size_t SCREEN_WIDTH = 640;
const size_t SCREEN_HEIGHT = 480;

class Program : public ILogic
{
public:
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
    void loadMedia();
    std::shared_ptr<Texture> loadTexture(std::string const& name, bool flag = false,
                                         Color const* color = nullptr);
    SDL_Rect getRect(int x, int y, int w, int h);

    uint8_t frame = 0;
    uint8_t delay = 0;
    size_t rotationDelay = 0;
    int direction = 10;
    size_t dudeHeight = 205;
    size_t dudeWidth = 265 / 4;
    int x = SCREEN_WIDTH / 2 - dudeWidth / 2;
    int y = SCREEN_HEIGHT / 2 - dudeHeight / 2;
    RotationData rotation = RotationData();

    Window* window;
    std::shared_ptr<Input> input;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Renderer> renderer;
    Subscriptions subs;
    MediaContainer images;
};
}
