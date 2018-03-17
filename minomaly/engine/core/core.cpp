#include "core.h"

using namespace Mino;

EngineCore::EngineCore(std::unique_ptr<SdlSubsystems>&& subsystems,
                       std::unique_ptr<IInputSystem>&& input,
                       std::unique_ptr<IWindowSystem>&& window,
                       std::unique_ptr<Application>&& app,
                       std::unique_ptr<IRenderSystem>&& renderer,
                       std::unique_ptr<IAudioSystem>&& audio,
                       std::unique_ptr<IPhysicsSystem>&& physicsSystem,
                       std::unique_ptr<IAssetSystem>&& assets,
                       std::shared_ptr<ILogService> const& logService,
                       std::shared_ptr<ITimeService> const& time)
    : subsystems(std::move(subsystems))
    , input(std::move(input))
    , window(std::move(window))
    , application(std::move(app))
    , renderer(std::move(renderer))
    , audioSystem(std::move(audio))
    , physicsSystem(std::move(physicsSystem))
    , assets(std::move(assets))
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
    auto inputSystem = Input::create();
    auto physicsSystem = PhysicsSystem::create();

    auto logService = Services::get<ILogService>();
    auto subsystems = SdlSubsystems::initialize(logService);
    auto audio = subsystems->subsystemStatus(SdlSubSystemType::SDL_mixer) == SdlStatus::Initialized
                     ? AudioSystem::create()
                     : std::make_unique<MuteAudioSystem>();

    auto windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS;
    auto window = WindowSystem::create(name.c_str(),
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       screenWidth,
                                       screenHeight,
                                       windowFlags);
    auto renderer = RenderSystem::create(*window);
    auto assets = AssetSystem::create(renderer.get());
    return std::make_shared<EngineCore>(std::move(subsystems),
                                        std::move(inputSystem),
                                        std::move(window),
                                        std::move(app),
                                        std::move(renderer),
                                        std::move(audio),
                                        std::move(physicsSystem),
                                        std::move(assets),
                                        logService,
                                        Services::get<ITimeService>());
}
