#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "audio_system.h"
#include "camera_component.h"
#include "input.h"
#include "log_service.h"
#include "observer.h"
#include "physics_system.h"
#include "render_system.h"
#include "scene.h"
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
class Scene;

class IEngineCore
{
public:
    typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
    typedef std::chrono::duration<float, std::milli> Milli;
    const float OneSecInMs = 1000.0;

    virtual ~IEngineCore() {}

    virtual void run() = 0;
    virtual void stop() = 0;

    virtual std::shared_ptr<IWindowSystem> getWindow() const = 0;
    virtual std::shared_ptr<IInputSystem> getInput() const = 0;
    virtual std::shared_ptr<IRenderSystem> getRenderer() const = 0;
    virtual std::shared_ptr<IAudioSystem> getAudio() const = 0;
    virtual std::shared_ptr<Scene> getScene() const = 0;
    virtual std::shared_ptr<ITimeService> getTime() const = 0;
    virtual std::shared_ptr<IPhysicsSystem> getPhysicsSystem() const = 0;
    virtual void setScene(std::shared_ptr<Scene> scene) = 0;

    virtual void setTargetFps(float f) = 0;

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
               std::shared_ptr<IAudioSystem> audioSystem,
               std::shared_ptr<IPhysicsSystem> physicsSystem,
               std::shared_ptr<ILogService> logService, std::shared_ptr<ITimeService> time);
    EngineCore(EngineCore const&) = delete;
    EngineCore(EngineCore&&) = delete;
    virtual ~EngineCore();

    EngineCore& operator=(EngineCore const&) = delete;
    EngineCore& operator=(EngineCore&&) = delete;

    virtual void run();
    virtual void stop();

    virtual std::shared_ptr<IWindowSystem> getWindow() const { return window; }
    virtual std::shared_ptr<IInputSystem> getInput() const { return input; }
    virtual std::shared_ptr<IRenderSystem> getRenderer() const { return renderer; }
    virtual std::shared_ptr<IAudioSystem> getAudio() const { return audioSystem; }
    virtual std::shared_ptr<ITimeService> getTime() const { return time; };
    virtual std::shared_ptr<Scene> getScene() const { return scene; }
    virtual std::shared_ptr<IPhysicsSystem> getPhysicsSystem() const { return physicsSystem; }
    virtual void setScene(std::shared_ptr<Scene> scene) { this->scene = scene; }

    virtual void setTargetFps(float f);

    virtual SdlStatus subsystemStatus(SdlSubSystemType type) const;
    virtual std::vector<SdlStatus>
    subsystemStatus(std::vector<SdlSubSystemType> const& types) const;

private:
    static std::shared_ptr<EngineCore> initCore(std::string const& name, size_t screenWidth,
                                            size_t screenHeight);
    static void setupMainCamera(std::shared_ptr<Scene>, std::shared_ptr<IRenderSystem>, float);
    template <class T> static void initScene(std::shared_ptr<EngineCore>, float screenHeight);

    void run(bool);
    void update();

    bool active = false;
    TimePoint lastUpdate;
    TimePoint lastFixedUpdate;
    Milli targetMsPerUpdate = Milli{OneSecInMs / 60.0};

    std::shared_ptr<SdlSubsystems> subsystems;
    std::shared_ptr<IInputSystem> input;
    std::shared_ptr<IWindowSystem> window;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<IRenderSystem> renderer;
    std::shared_ptr<IAudioSystem> audioSystem;
    std::shared_ptr<IPhysicsSystem> physicsSystem;
    std::shared_ptr<ILogService> logService;
    std::shared_ptr<ITimeService> time;
    ISubscription sub;
};

template <typename TLogic>
std::shared_ptr<EngineCore> EngineCore::create(std::string const& name, size_t screenWidth,
                                               size_t screenHeight)
{
    auto core = initCore(name, screenWidth, screenHeight);
    initScene<TLogic>(core, (float)screenHeight);
    return core;
}

template <typename TLogic>
void EngineCore::initScene(std::shared_ptr<EngineCore> core, float screenHeight)
{
    auto scene = std::make_shared<TLogic>(core);
    core->setScene(scene);
    setupMainCamera(scene, core->renderer, screenHeight);
}

} // namespace Mino
