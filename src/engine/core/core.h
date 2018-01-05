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

namespace Mino
{

class IWindow;
class IRenderSystem;

class IEngineCore
{
public:
    virtual ~IEngineCore() {}

    virtual void run() = 0;
    virtual void stop() = 0;
};

class Core : public IEngineCore
{
public:
    template <typename TLogic>
    static std::shared_ptr<Core> create(std::string const& name, size_t screenWidth,
                                        size_t screenHeight);

    Core(std::shared_ptr<SdlSubsystems> subsystems, std::shared_ptr<IInputSystem> input,
         std::unique_ptr<IWindow>&& window, std::shared_ptr<IRenderSystem> renderer);
    Core(Core const&) = delete;
    Core(Core&&) = delete;
    virtual ~Core();

    Core& operator=(Core const&) = delete;
    Core& operator=(Core&&) = delete;

    virtual void run();
    virtual void stop();

    IWindow* getWindow() { return window.get(); }
    std::shared_ptr<IInputSystem> getInput() { return input; }
    std::shared_ptr<IRenderSystem> getRenderer() { return renderer; }
    std::shared_ptr<ILogicSystem> getLogic() { return logic; }
    void setLogic(std::shared_ptr<ILogicSystem> logic) { this->logic = logic; }

private:
    bool active = false;
    std::shared_ptr<SdlSubsystems> subsystems;
    std::shared_ptr<IInputSystem> input;
    std::unique_ptr<IWindow> window;
    std::shared_ptr<ILogicSystem> logic;
    std::shared_ptr<IRenderSystem> renderer;
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
} // namespace Mino
