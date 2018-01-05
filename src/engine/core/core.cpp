#include "core.h"

Core::Core(std::shared_ptr<SdlSubsystems> subsystems, std::shared_ptr<IInputSystem> input,
           std::unique_ptr<IWindow>&& window, std::shared_ptr<IRenderSystem> renderer)
    : subsystems(subsystems), input(input), window(std::move(window)), renderer(renderer)
{
    sub = input->onQuit([&](auto const&) { active = false; });
}

Core::~Core() {}

void Core::run()
{
    active = true;
    logic->start();
    while (active)
    {
        renderer->clear();

        input->update();
        logic->update();
        renderer->update();
    }
}

void Core::stop() { active = false; }
