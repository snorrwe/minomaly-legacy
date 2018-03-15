#include "core.h"

using namespace Mino;

EngineCore::EngineCore(std::shared_ptr<SdlSubsystems> const& subsystems,
                       std::shared_ptr<IInputSystem> const& input,
                       std::shared_ptr<IWindowSystem> const& window,
                       std::shared_ptr<Application> const& app,
                       std::shared_ptr<IRenderSystem> const& renderer,
                       std::shared_ptr<IAudioSystem> const& audio,
                       std::shared_ptr<IPhysicsSystem> const& physicsSystem,
                       std::shared_ptr<IAssetSystem> const& assets,
                       std::shared_ptr<ILogService> const& logService,
                       std::shared_ptr<ITimeService> const& time)
    : subsystems(subsystems)
    , input(input)
    , window(window)
    , application(app)
    , renderer(renderer)
    , audioSystem(audio)
    , physicsSystem(physicsSystem)
    , assets(assets)
    , logService(logService)
    , time(time)
{
    sub = input->onQuit([&](auto const&) { active = false; });
}

EngineCore::~EngineCore() { application.reset(); }

void EngineCore::run() try
{
    active = true;
    lastUpdate = std::chrono::system_clock::now();
    lastFixedUpdate = std::chrono::system_clock::now();
    auto lag = Milli{0.0};

    time->update(lastUpdate);
    application->start();
    while (active)
    {
        auto now = std::chrono::system_clock::now();
        auto elapsed = now - lastFixedUpdate;
        lastFixedUpdate = now;
        lastUpdate = now;
        lag += elapsed;
        input->update();
        while (lag >= targetMsPerUpdate)
        {
            update();
            lag -= targetMsPerUpdate;
            lastUpdate = std::chrono::system_clock::now();
        }
        physicsSystem->update();
        renderer->update();
    }
}
catch (std::exception& exc)
{
    logService->error("Unexpected exception was thrown while running Minomaly!");
    logService->error(exc.what());
}
catch (...)
{
    logService->error("Unknown error happened while running Minomaly!");
}

void EngineCore::update()
{
    time->update(lastUpdate);
    application->update();
    application->updateGameObjects();
}

void EngineCore::stop() { active = false; }

SdlStatus EngineCore::subsystemStatus(SdlSubSystemType type) const
{
    return subsystems->subsystemStatus(type);
}

void EngineCore::setTargetFps(float f) { targetMsPerUpdate = Milli{OneSecInMs / f}; }

std::shared_ptr<EngineCore> EngineCore::initCore(std::string const& name,
                                                 size_t screenWidth,
                                                 size_t screenHeight,
                                                 std::shared_ptr<Application> const& app)
{
    auto logService = Services::get<ILogService>();
    auto time = Services::get<ITimeService>();
    auto subsystems = SdlSubsystems::initialize(logService);
    auto audio = subsystems->subsystemStatus(SdlSubSystemType::SDL_mixer) == SdlStatus::Initialized
                     ? AudioSystem::create()
                     : std::make_shared<MuteAudioSystem>();
    auto inp = Input::create();
    auto window
        = WindowSystem::create(name.c_str(),
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               screenWidth,
                               screenHeight,
                               SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
    auto renderer = RenderSystem::create(*window);
    auto physics = PhysicsSystem::create();
    auto assets = AssetSystem::create(renderer.get());

    return std::make_shared<EngineCore>(
        subsystems, inp, window, app, renderer, audio, physics, assets, logService, time);
}
