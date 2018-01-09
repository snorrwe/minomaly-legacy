#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "audio_system.h"
#include "input.h"
#include "observer.h"
#include "render_system.h"
#include "scene.h"
#include "sdl_subsystems.h"
#include "surface.h"
#include "window.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace Mino
{

class IWindowSystem;
class IRenderSystem;
class Scene;

class IEngineCore
{
public:
    virtual ~IEngineCore() {}

    virtual void run() = 0;
    virtual void stop() = 0;

    virtual std::shared_ptr<IWindowSystem> getWindow() = 0;
    virtual std::shared_ptr<IInputSystem> getInput() = 0;
    virtual std::shared_ptr<IRenderSystem> getRenderer() = 0;
    virtual std::shared_ptr<IAudioSystem> getAudio() = 0;
    virtual std::shared_ptr<Scene> getLogic() = 0;
    virtual void setLogic(std::shared_ptr<Scene> logic) = 0;
};

class Core : public IEngineCore
{
public:
    template <typename TLogic>
    static std::shared_ptr<Core> create(std::string const& name, size_t screenWidth,
                                        size_t screenHeight);

    Core(std::shared_ptr<SdlSubsystems> subsystems, std::shared_ptr<IInputSystem> input,
         std::shared_ptr<IWindowSystem> window, std::shared_ptr<IRenderSystem> renderer,
         std::shared_ptr<IAudioSystem> audioSystem);
    Core(Core const&) = delete;
    Core(Core&&) = delete;
    virtual ~Core();

    Core& operator=(Core const&) = delete;
    Core& operator=(Core&&) = delete;

    virtual void run();
    virtual void stop();

    virtual std::shared_ptr<IWindowSystem> getWindow() { return window; }
    virtual std::shared_ptr<IInputSystem> getInput() { return input; }
    virtual std::shared_ptr<IRenderSystem> getRenderer() { return renderer; }
    virtual std::shared_ptr<IAudioSystem> getAudio() { return audioSystem; }
    virtual std::shared_ptr<Scene> getLogic() { return logic; }
    virtual void setLogic(std::shared_ptr<Scene> logic) { this->logic = logic; }

private:
    void _run();

    bool active = false;
    std::shared_ptr<SdlSubsystems> subsystems;
    std::shared_ptr<IInputSystem> input;
    std::shared_ptr<IWindowSystem> window;
    std::shared_ptr<Scene> logic;
    std::shared_ptr<IRenderSystem> renderer;
    std::shared_ptr<IAudioSystem> audioSystem;
    ISubscription sub;
};

template <typename TLogic>
std::shared_ptr<Core> Core::create(std::string const& name, size_t screenWidth, size_t screenHeight)
{
    auto subsystems = SdlSubsystems::initialize();
    auto inp = Input::create();
    auto window =
        WindowSystem::create(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    auto renderer = RenderSystem::create(*window);
    auto audio = AudioSystem::create();
    auto core = std::make_shared<Core>(subsystems, inp, window, renderer, audio);
    auto logic = std::make_shared<TLogic>(core);
    core->setLogic(logic);
    return core;
}

} // namespace Mino
