#include "core.h"

using namespace Mino;

Core::Core(std::shared_ptr<SdlSubsystems> subsystems, std::shared_ptr<IInputSystem> input,
           std::unique_ptr<IWindow>&& window, std::shared_ptr<IRenderSystem> renderer,
           std::shared_ptr<IAudioSystem> audioSystem)
    : subsystems(subsystems),
      input(input),
      window(std::move(window)),
      renderer(renderer),
      audioSystem(audioSystem)
{
    sub = input->onQuit([&](auto const&) { active = false; });
}

Core::~Core() {}

void Core::run()
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

void Core::_run()
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

void Core::stop() { active = false; }
