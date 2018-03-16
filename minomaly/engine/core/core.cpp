#include "core.h"

using namespace Mino;

EngineCore::EngineCore(std::unique_ptr<SdlSubsystems>&& subsystems,
                       std::unique_ptr<IInputSystem>&& input,
                       std::shared_ptr<IWindowSystem> const& window,
                       std::unique_ptr<Application>&& app,
                       std::shared_ptr<IRenderSystem> const& renderer,
                       std::shared_ptr<IAudioSystem> const& audio,
                       std::shared_ptr<IPhysicsSystem> const& physicsSystem,
                       std::shared_ptr<IAssetSystem> const& assets,
                       std::shared_ptr<ILogService> const& logService,
                       std::shared_ptr<ITimeService> const& time)
    : subsystems(std::move(subsystems))
    , input(std::move(input))
    , window(window)
    , application(std::move(app))
    , renderer(renderer)
    , audioSystem(audio)
    , physicsSystem(physicsSystem)
    , assets(assets)
    , logService(logService)
    , time(time)
{
    quitSub = this->input->onQuit([&](auto const&) { active = false; });
}

EngineCore::~EngineCore() { application.reset(); }

void EngineCore::run()
{
    try
    {
        active = true;
        runApplication();
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
}

void EngineCore::runApplication()
{
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
            time->update(lastUpdate);
            updateLogic();
            lag -= targetMsPerUpdate;
            lastUpdate = std::chrono::system_clock::now();
        }
        physicsSystem->update();
        renderer->update();
    }
    application->stop();
}

void EngineCore::updateLogic()
{
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
                                                 std::unique_ptr<Application>&& app)
{
    auto logService = Services::get<ILogService>();
    auto time = Services::get<ITimeService>();
    auto subsystems = SdlSubsystems::initialize(logService);
    auto audio = subsystems->subsystemStatus(SdlSubSystemType::SDL_mixer) == SdlStatus::Initialized
                     ? AudioSystem::create()
                     : std::make_shared<MuteAudioSystem>();
    auto window
        = WindowSystem::create(name.c_str(),
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               screenWidth,
                               screenHeight,
                               SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
    auto renderer = RenderSystem::create(*window);
    auto assets = AssetSystem::create(renderer.get());
    return std::make_shared<EngineCore>(std::move(subsystems),
                                        Input::create(),
                                        window,
                                        std::move(app),
                                        renderer,
                                        audio,
                                        PhysicsSystem::create(),
                                        assets,
                                        logService,
                                        time);
}
