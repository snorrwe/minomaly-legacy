#include "core.h"

using namespace Mino;

EngineCore::EngineCore(std::shared_ptr<SdlSubsystems> subsystems, std::shared_ptr<IInputSystem> input,
	std::shared_ptr<IWindowSystem> window, std::shared_ptr<IRenderSystem> renderer,
           std::shared_ptr<IAudioSystem> audioSystem)
    : subsystems(subsystems),
      input(input),
      window(std::move(window)),
      renderer(renderer),
      audioSystem(audioSystem)
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
    logic->start();
    while (active)
    {
        renderer->clear();

        input->update();
        logic->update();
        logic->updateGameObjects();

        renderer->update();
    }
}

void EngineCore::stop() { active = false; }
