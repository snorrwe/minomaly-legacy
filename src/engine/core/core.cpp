#include "core.h"

bool Core::isInitialized = false;

void Core::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error:\n" << SDL_GetError() << std::endl;
        throw std::runtime_error("SDL could not initialize!");
    }
    auto imgFlags = IMG_INIT_PNG;
    if (!IMG_Init(imgFlags) & imgFlags)
    {
        std::cout << "SDL_image could not initialize! SDL_image Error:\n"
                  << IMG_GetError() << std::endl;
        throw std::runtime_error("IMG could not initialize!");
    }
    isInitialized = true;
}

Core::Core(std::shared_ptr<Input> inp, std::unique_ptr<Window>&& win,
           std::shared_ptr<Renderer> renderer)
    : input(inp), window(std::move(win)), renderer(renderer)
{
    sub = input->onQuit([&](auto const&) { active = false; });
}

Core::~Core()
{
    sub.unsubscribe();
    window.release();
    IMG_Quit();
    SDL_Quit();
    isInitialized = false;
}

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
