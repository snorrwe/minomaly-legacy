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
#include <vector>

namespace Spritesheet
{

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

    Window* window;
    std::shared_ptr<Input> input;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Renderer> renderer;
    Subscriptions subs;
    MediaContainer images;

    int x = 0;
    int y = 0;
    int vx = 0;
    int vy = 0;
};
}
