#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "application.h"
#include "asset_system.h"
#include "audio_system.h"
#include "camera_component.h"
#include "input_system.h"
#include "log_service.h"
#include "observer.h"
#include "physics_system.h"
#include "render_system.h"
#include "sdl_subsystems.h"
#include "services.h"
#include "surface.h"
#include "time_service.h"
#include "window.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace Mino
{

class IWindowSystem;
class IRenderSystem;
class IPhysicsSystem;
class Application;
class IAssetSystem;

class IEngineCore
{
public:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    using Milli = std::chrono::duration<float, std::milli>;
    const float OneSecInMs = 1000.0;

    virtual ~IEngineCore() {}

    virtual void run() = 0;
    virtual void stop() = 0;

    virtual IWindowSystem* getWindow() const = 0;
    virtual IInputSystem* getInput() const = 0;
    virtual IRenderSystem* getRenderer() const = 0;
    virtual IAudioSystem* getAudio() const = 0;
    virtual Application* getApplication() const = 0;
    virtual IPhysicsSystem* getPhysicsSystem() const = 0;
    virtual IAssetSystem* getAssets() const = 0;

    virtual void setTargetFps(float f) = 0;

    virtual SdlStatus subsystemStatus(SdlSubSystemType) const = 0;
};

class EngineCore : public IEngineCore
{
public:
    template <typename TLogic>
    static std::shared_ptr<EngineCore> create(std::string const& name, size_t screenWidth,
                                              size_t screenHeight);

    EngineCore(std::shared_ptr<SdlSubsystems> subsystems, std::shared_ptr<IInputSystem> input,
               std::shared_ptr<IWindowSystem> window, std::shared_ptr<Application> app,
               std::shared_ptr<IRenderSystem> renderer, std::shared_ptr<IAudioSystem> audioSystem,
               std::shared_ptr<IPhysicsSystem> physicsSystem, std::shared_ptr<IAssetSystem> assets,
               std::shared_ptr<ILogService> logService, std::shared_ptr<ITimeService> time);
    EngineCore(EngineCore const&) = delete;
    EngineCore(EngineCore&&) = delete;
    virtual ~EngineCore();

    EngineCore& operator=(EngineCore const&) = delete;
    EngineCore& operator=(EngineCore&&) = delete;

    virtual void run();
    virtual void stop();

    virtual IWindowSystem* getWindow() const { return window.get(); }
    virtual IInputSystem* getInput() const { return input.get(); }
    virtual IRenderSystem* getRenderer() const { return renderer.get(); }
    virtual IAudioSystem* getAudio() const { return audioSystem.get(); }
    virtual Application* getApplication() const { return application.get(); }
    virtual IPhysicsSystem* getPhysicsSystem() const { return physicsSystem.get(); }
    virtual IAssetSystem* getAssets() const { return assets.get(); }

    virtual void setTargetFps(float f);

    virtual SdlStatus subsystemStatus(SdlSubSystemType type) const;

private:
    static std::shared_ptr<EngineCore> initCore(std::string const& name, size_t screenWidth,
                                                size_t screenHeight,
                                                std::shared_ptr<Application> const& app);

    void run(bool);
    void update();

    bool active = false;
    TimePoint lastUpdate;
    TimePoint lastFixedUpdate;
    Milli targetMsPerUpdate = Milli{OneSecInMs / 128.0};

    std::shared_ptr<SdlSubsystems> subsystems;
    std::shared_ptr<IInputSystem> input;
    std::shared_ptr<IWindowSystem> window;
    std::shared_ptr<Application> application;
    std::shared_ptr<IRenderSystem> renderer;
    std::shared_ptr<IAudioSystem> audioSystem;
    std::shared_ptr<IPhysicsSystem> physicsSystem;
    std::shared_ptr<IAssetSystem> assets;
    std::shared_ptr<ILogService> logService;
    std::shared_ptr<ITimeService> time;
    ISubscription sub;
};

template <typename TLogic>
std::shared_ptr<EngineCore> EngineCore::create(std::string const& name, size_t screenWidth,
                                               size_t screenHeight)
{
    auto application = std::make_shared<TLogic>();
    auto core = initCore(name, screenWidth, screenHeight, application);
    application->setEngineCore(core.get());
    application->initMainCamera(*core->renderer, screenHeight);
    return core;
}

} // namespace Mino
