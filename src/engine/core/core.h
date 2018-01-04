#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "input.h"
#include "logic.h"
#include "observer.h"
#include "renderer.h"
#include "sdl_subsystems.h"
#include "surface.h"
#include "window.h"
#include <iostream>
#include <memory>
#include <string>

class Window;
class Renderer;

class Core
{
public:
    template <typename TLogic>
    static std::shared_ptr<Core> create(std::string const& name, size_t screenWidth,
                                        size_t screenHeight);

    Core(std::shared_ptr<SdlSubsystems> subsystems, std::shared_ptr<Input> input,
         std::unique_ptr<Window>&& window, std::shared_ptr<Renderer> renderer);
    Core(Core const&) = delete;
    Core(Core&&) = delete;
    ~Core();

    Core& operator=(Core const&) = delete;
    Core& operator=(Core&&) = delete;

    void run();
    void stop();

    Window* getWindow() { return window.get(); }
    std::shared_ptr<Input> getInput() { return input; }
    std::shared_ptr<Renderer> getRenderer() { return renderer; }
    std::shared_ptr<ILogic> getLogic() { return logic; }
    void setLogic(std::shared_ptr<ILogic> logic) { this->logic = logic; }

private:
    bool active = false;
    std::shared_ptr<SdlSubsystems> subsystems;
    std::shared_ptr<Input> input;
    std::unique_ptr<Window> window;
    std::shared_ptr<ILogic> logic;
    std::shared_ptr<Renderer> renderer;
    ISubscription sub;
};

template <typename TLogic>
std::shared_ptr<Core> Core::create(std::string const& name, size_t screenWidth, size_t screenHeight)
{
    auto subsystems = SdlSubsystems::initialize();
    auto inp = Input::create();
    auto win = Window::create(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    auto renderer = Renderer::create(win->getWindow());
    auto core = std::make_shared<Core>(subsystems, inp, std::move(win), renderer);
    auto logic = std::make_shared<TLogic>(core);
    core->setLogic(logic);
    return core;
}
