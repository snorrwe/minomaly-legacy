#include "sdl_subsystems.h"

using namespace Mino;

bool SdlSubsystems::isInitialized = false;
SdlSubsystems* SdlSubsystems::instance = nullptr;
std::array<SdlStatus, static_cast<int>(SdlSubSystemType::COUNT)> SdlSubsystems::status{};

SdlSubsystems::~SdlSubsystems()
{
    std::cout << "Quitting subsystems\n" << std::endl;
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    isInitialized = false;
}

std::shared_ptr<SdlSubsystems> SdlSubsystems::initialize()
{
    if (isInitialized) return std::shared_ptr<SdlSubsystems>(instance);
    std::cout << "Initializing SDL subsystems\n" << std::endl;
    Init_SDL();
    Init_SDL_image();
    Init_SDL_ttf();
    Init_SDL_mixer();
    std::cout << "Done initialising SDL subsystems\n-------------------------------------\n";
    auto result = std::make_shared<SdlSubsystems>();
    instance = result.get();
    isInitialized = true;
    return result;
}

void SdlSubsystems::Init_SDL()
{
    status[static_cast<int>(SdlSubSystemType::SDL)] =
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ? SdlStatus::Error : SdlStatus::Initialized;
    if (status[static_cast<int>(SdlSubSystemType::SDL)] == SdlStatus::Error)
    {
        std::cout << "SDL could not initialize! SDL_Error:\n" << SDL_GetError() << std::endl;
    }
}

void SdlSubsystems::Init_SDL_image()
{
    auto imgFlags = IMG_INIT_PNG;

    status[static_cast<int>(SdlSubSystemType::SDL_image)] =
        (!IMG_Init(imgFlags)) & imgFlags ? SdlStatus::Error : SdlStatus::Initialized;
    if (status[static_cast<int>(SdlSubSystemType::SDL_image)] == SdlStatus::Error)
    {
        std::cout << "SDL_image could not initialize! SDL_image Error:\n"
                  << IMG_GetError() << std::endl;
    }
}

void SdlSubsystems::Init_SDL_ttf()
{
    status[static_cast<int>(SdlSubSystemType::SDL_ttf)] =
        TTF_Init() < 0 ? SdlStatus::Error : SdlStatus::Initialized;
    if (status[static_cast<int>(SdlSubSystemType::SDL_ttf)] == SdlStatus::Error)
    {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: \n"
                  << TTF_GetError() << std::endl;
    }
}

void SdlSubsystems::Init_SDL_mixer()
{
    status[static_cast<int>(SdlSubSystemType::SDL_mixer)] =
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ? SdlStatus::Error
                                                              : SdlStatus::Initialized;
    if (status[static_cast<int>(SdlSubSystemType::SDL_mixer)] == SdlStatus::Error)
    {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: \n"
                  << Mix_GetError() << std::endl;
    }
}

SdlStatus SdlSubsystems::subsystemStatus(SdlSubSystemType type) const
{
    return status[static_cast<int>(type)];
}

std::vector<SdlStatus>
SdlSubsystems::subsystemStatus(std::vector<SdlSubSystemType> const& types) const
{
    std::vector<SdlStatus> result{};
    for (auto i = types.begin(); i != types.end(); ++i)
    {
        result.push_back(subsystemStatus(*i));
    }
    return result;
}
