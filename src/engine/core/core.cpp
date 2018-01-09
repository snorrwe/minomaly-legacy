#include "core.h"

using namespace Mino;

EngineCore::EngineCore(std::shared_ptr<SdlSubsystems> subsystems,
                       std::shared_ptr<IInputSystem> input,
                       std::shared_ptr<IWindowSystem> windowSytem,
                       std::shared_ptr<IRenderSystem> renderer,
                       std::shared_ptr<IAudioSystem> audioSystem,
                       std::shared_ptr<ILogService> logService)
    : subsystems(subsystems),
      input(input),
      window(windowSytem),
      renderer(renderer),
      audioSystem(audioSystem),
      logService(logService)
{
    sub = input->onQuit([&](auto const&) { active = false; });
}

EngineCore::~EngineCore() {}

void EngineCore::run()
{
    try
    {
        _run();
    }
    catch (std::runtime_error& exc)
    {
        std::cout << "Unexpected error happened while running the Game! " << exc.what()
                  << std::endl;
    }
    catch (...)
    {
        std::cout << "Unknown error happened while running the Game! " << std::endl;
    }
}

void EngineCore::_run()
{
    active = true;
    scene->start();
    while (active)
    {
        renderer->clear();
        update();
        renderer->update();
    }
}

void EngineCore::update()
{
    input->update();
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
