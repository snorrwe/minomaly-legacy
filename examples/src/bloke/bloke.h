#pragma once
#include "mino.h"
#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace BlokeGame
{

const size_t SCREEN_WIDTH = 640;
const size_t SCREEN_HEIGHT = 480;

class Program : public Mino::ILogicSystem
{
public:
    typedef std::array<Mino::ISubscription, 1> Subscriptions;
    typedef std::array<std::shared_ptr<Mino::Texture>, 2> MediaContainer;

    Program(std::shared_ptr<Mino::Core> core);
    Program(Program const&) = delete;
    Program(Program&&) = delete;
    ~Program();

    Program& operator=(Program const&) = delete;
    Program& operator=(Program&&) = delete;

    virtual void update();
    virtual void start();

private:
    void loadMedia();
    void handleKey(SDL_Keycode const&, int vx, int vy);
    std::shared_ptr<Mino::Texture> loadTexture(std::string const& name, bool flag = false,
                                               Mino::Color const* color = nullptr);

    std::shared_ptr<Mino::IInputSystem> input;
    std::shared_ptr<Mino::ICamera> camera;
    std::shared_ptr<Mino::IRenderSystem> renderer;
    Subscriptions subs;
    MediaContainer images;

    int x = 0, y = 0, vx = 0, vy = 0;
    uint8_t delay = 0;
};
} // namespace BlokeGame
