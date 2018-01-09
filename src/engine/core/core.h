#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "audio_system.h"
#include "input.h"
#include "log_service.h"
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
    virtual std::shared_ptr<Scene> getScene() = 0;
    virtual void setScene(std::shared_ptr<Scene> scene) = 0;

    virtual SdlStatus subsystemStatus(SdlSubSystemType) const = 0;
    virtual std::vector<SdlStatus> subsystemStatus(std::vector<SdlSubSystemType> const&) const = 0;
};

class EngineCore : public IEngineCore
{
public:
    template <typename TLogic>
    static std::shared_ptr<EngineCore> create(std::string const& name, size_t screenWidth,
                                              size_t screenHeight);

    EngineCore(std::shared_ptr<SdlSubsystems> subsystems, std::shared_ptr<IInputSystem> input,
               std::shared_ptr<IWindowSystem> window, std::shared_ptr<IRenderSystem> renderer,
               std::shared_ptr<IAudioSystem> audioSystem, std::shared_ptr<ILogService> logService);
    EngineCore(EngineCore const&) = delete;
    EngineCore(EngineCore&&) = delete;
    virtual ~EngineCore();

    EngineCore& operator=(EngineCore const&) = delete;
    EngineCore& operator=(EngineCore&&) = delete;

    virtual void run();
    virtual void stop();

    virtual std::shared_ptr<IWindowSystem> getWindow() { return window; }
    virtual std::shared_ptr<IInputSystem> getInput() { return input; }
    virtual std::shared_ptr<IRenderSystem> getRenderer() { return renderer; }
    virtual std::shared_ptr<IAudioSystem> getAudio() { return audioSystem; }
    virtual std::shared_ptr<Scene> getScene() { return scene; }
    virtual void setScene(std::shared_ptr<Scene> scene) { this->scene = scene; }

    virtual SdlStatus subsystemStatus(SdlSubSystemType type) const;
    virtual std::vector<SdlStatus>
    subsystemStatus(std::vector<SdlSubSystemType> const& types) const;

private:
    void _run();
    void update();

    bool active = false;
    std::shared_ptr<SdlSubsystems> subsystems;
    std::shared_ptr<IInputSystem> input;
    std::shared_ptr<IWindowSystem> window;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<IRenderSystem> renderer;
    std::shared_ptr<IAudioSystem> audioSystem;
    std::shared_ptr<ILogService> logService;
    ISubscription sub;
};

template <typename TLogic>
std::shared_ptr<EngineCore> EngineCore::create(std::string const& name, size_t screenWidth,
                                               size_t screenHeight)
{
    auto logService = LogService::create("mino_debug.log");
    auto subsystems = SdlSubsystems::initialize(logService);
    auto audio = subsystems->subsystemStatus(SdlSubSystemType::SDL_mixer) == SdlStatus::Initialized
                     ? AudioSystem::create()
                     : std::make_shared<MuteAudioSystem>();
    auto inp = Input::create();
    auto window =
        WindowSystem::create(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    auto renderer = RenderSystem::create(*window);
    auto core = std::make_shared<EngineCore>(subsystems, inp, window, renderer, audio, logService);
    auto scene = std::make_shared<TLogic>(core);
    core->setScene(scene);
    return core;
}

} // namespace Mino
