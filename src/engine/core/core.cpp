#include "core.h"

using namespace Mino;

EngineCore::EngineCore(std::shared_ptr<SdlSubsystems> subsystems,
                       std::shared_ptr<IInputSystem> input, std::shared_ptr<IWindowSystem> window,
                       std::shared_ptr<IRenderSystem> renderer, std::shared_ptr<IAudioSystem> audio,
                       std::shared_ptr<IPhysicsSystem> physicsSystem,
                       std::shared_ptr<ILogService> logService, std::shared_ptr<ITimeService> time)
    : subsystems(subsystems),
      input(input),
      window(window),
      renderer(renderer),
      audioSystem(audio),
      physicsSystem(physicsSystem),
      logService(logService),
      time(time)
{
    sub = input->onQuit([&](auto const&) { active = false; });
}

EngineCore::~EngineCore() { scene.reset(); }

void EngineCore::run()
{
    try
    {
        run(true);
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

void EngineCore::run(bool)
{
    active = true;
    lastUpdate = std::chrono::system_clock::now();
    lastFixedUpdate = std::chrono::system_clock::now();
    Milli lag{0.0};

    time->update(lastUpdate);
    scene->start();
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

void EngineCore::update()
{
    time->update(lastUpdate);
    scene->update();
    scene->updateGameObjects();
}

void EngineCore::stop() { active = false; }

SdlStatus EngineCore::subsystemStatus(SdlSubSystemType type) const
{
    return subsystems->subsystemStatus(type);
}

std::vector<SdlStatus> EngineCore::subsystemStatus(std::vector<SdlSubSystemType> const& types) const
{
    return subsystems->subsystemStatus(types);
}

void EngineCore::setTargetFps(float f) { targetMsPerUpdate = Milli{OneSecInMs / f}; }

std::shared_ptr<EngineCore> EngineCore::initCore(std::string const& name, size_t screenWidth,
                                                 size_t screenHeight)
{
    auto logService = Services::get<ILogService>();
    auto time = Services::get<ITimeService>();
    auto subsystems = SdlSubsystems::initialize(logService);
    auto audio = subsystems->subsystemStatus(SdlSubSystemType::SDL_mixer) == SdlStatus::Initialized
                     ? AudioSystem::create()
                     : std::make_shared<MuteAudioSystem>();
    auto inp = Input::create();
    auto window = WindowSystem::create(
        name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
    auto renderer = RenderSystem::create(*window);
    auto physics = PhysicsSystem::create();

    return std::make_shared<EngineCore>(subsystems, inp, window, renderer, audio, physics,
                                        logService, time);
}
