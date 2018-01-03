#pragma once
#include "SDL.h"
#include "camera.h"
#include "color.h"
#include "core.h"
#include "input.h"
#include "surface.h"
#include "window.h"
#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace Spritesheet
{

const size_t SCREEN_WIDTH = 640;
const size_t SCREEN_HEIGHT = 480;

class Program : public ILogic
{
public:
    typedef std::array<ISubscription, 1> Subscriptions;
    typedef std::array<std::shared_ptr<Texture>, 2> MediaContainer;

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

    Window* window;
    std::shared_ptr<Input> input;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Renderer> renderer;
    Subscriptions subs;
    MediaContainer images;

    int x = 0, y = 0, vx = 0, vy = 0;
    uint8_t wait = 0;
    uint8_t alpha;
};
}
