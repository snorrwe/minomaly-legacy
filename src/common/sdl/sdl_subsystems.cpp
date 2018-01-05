#include "sdl_subsystems.h"

using namespace Mino;

bool SdlSubsystems::isInitialized = false;
SdlSubsystems* SdlSubsystems::instance = nullptr;

SdlSubsystems::~SdlSubsystems()
{
    std::cout << "Quitting subsystems\n" << std::endl;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    isInitialized = false;
}

std::shared_ptr<SdlSubsystems> SdlSubsystems::initialize()
{
    if (isInitialized) return std::shared_ptr<SdlSubsystems>(instance);
    std::cout << "Initializing SDL subsystems\n" << std::endl;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error:\n" << SDL_GetError() << std::endl;
        throw std::runtime_error("SDL could not initialize!");
    }
    auto imgFlags = IMG_INIT_PNG;
    if ((!IMG_Init(imgFlags)) & imgFlags)
    {
        std::cout << "SDL_image could not initialize! SDL_image Error:\n"
                  << IMG_GetError() << std::endl;
        throw std::runtime_error("SDL_image could not initialize!");
    }
    if (TTF_Init() < 0)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        throw std::runtime_error("SDL_ttf could not initialize!");
    }
    std::cout << "Successfully initialised SDL subsystems\n-------------------------------------\n";
    auto result = std::make_shared<SdlSubsystems>();
    instance = result.get();
    isInitialized = true;
    return result;
}
