#pragma once
#include "mino.h"
#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace Walk
{

const size_t SCREEN_WIDTH = 640;
const size_t SCREEN_HEIGHT = 480;

class Program : public Mino::Scene
{
public:
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
    void loadMedia();
    std::shared_ptr<Mino::Texture> loadTexture(std::string const& name, bool flag = false,
                                               Mino::Color const* color = nullptr);
    SDL_Rect getRect(int x, int y, int w, int h);

    uint8_t frame = 0;
    uint8_t delay = 0;
    size_t dudeHeight = 205;
    size_t dudeWidth = 265 / 4;
    int x = SCREEN_WIDTH / 2 - dudeWidth / 2;
    int y = SCREEN_HEIGHT / 2 - dudeHeight / 2;

    std::shared_ptr<Mino::IInputSystem> input;
    std::shared_ptr<Mino::ICamera> camera;
    std::shared_ptr<Mino::IRenderSystem> renderer;
    Subscriptions subs;
    MediaContainer images;
};
} // namespace Walk
