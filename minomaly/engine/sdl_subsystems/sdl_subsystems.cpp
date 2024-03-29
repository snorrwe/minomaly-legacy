#include "sdl_subsystems.h"

using namespace Mino;

bool SdlSubsystems::isInitialized = false;
std::array<SdlStatus, static_cast<int>(SdlSubSystemType::COUNT)> SdlSubsystems::status{};

SdlSubsystems::SdlSubsystems(std::shared_ptr<ILogService> logService)
    : logService(logService)
{
    if (isInitialized)
    {
        throw std::runtime_error(
            "No more than one instance of SdlSubsystems may be present at a time");
    }
    isInitialized = true;
}

SdlSubsystems::~SdlSubsystems()
{
    logService->debug("Quitting subsystems\n");
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    isInitialized = false;
}

std::unique_ptr<SdlSubsystems> SdlSubsystems::initialize(std::shared_ptr<ILogService> logService)
{
    logService->debug("Initializing SDL subsystems\n");
    Init_SDL(logService);
    Init_SDL_image(logService);
    Init_SDL_ttf(logService);
    Init_SDL_mixer(logService);
    logService->debug("Done initialising SDL subsystems\n-------------------------------------\n");
    return std::make_unique<SdlSubsystems>(logService);
}

void SdlSubsystems::Init_SDL(std::shared_ptr<ILogService> logService)
{
    status[static_cast<int>(SdlSubSystemType::SDL)]
        = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ? SdlStatus::Error : SdlStatus::Initialized;
    if (status[static_cast<int>(SdlSubSystemType::SDL)] == SdlStatus::Error)
    {
        logService->error("SDL could not initialize! SDL_Error:\n");
        logService->error(SDL_GetError());
    }
}

void SdlSubsystems::Init_SDL_image(std::shared_ptr<ILogService> logService)
{
    auto imgFlags = IMG_INIT_PNG;

    status[static_cast<int>(SdlSubSystemType::SDL_image)]
        = (!IMG_Init(imgFlags)) & imgFlags ? SdlStatus::Error : SdlStatus::Initialized;
    if (status[static_cast<int>(SdlSubSystemType::SDL_image)] == SdlStatus::Error)
    {
        logService->error("SDL_image could not initialize! SDL_image Error:\n");
        logService->error(IMG_GetError());
    }
}

void SdlSubsystems::Init_SDL_ttf(std::shared_ptr<ILogService> logService)
{
    status[static_cast<int>(SdlSubSystemType::SDL_ttf)]
        = TTF_Init() < 0 ? SdlStatus::Error : SdlStatus::Initialized;
    if (status[static_cast<int>(SdlSubSystemType::SDL_ttf)] == SdlStatus::Error)
    {
        logService->error("SDL_ttf could not initialize! SDL_ttf Error: \n");
        logService->error(TTF_GetError());
    }
}

void SdlSubsystems::Init_SDL_mixer(std::shared_ptr<ILogService> logService)
{
    status[static_cast<int>(SdlSubSystemType::SDL_mixer)]
        = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ? SdlStatus::Error
                                                                : SdlStatus::Initialized;
    if (status[static_cast<int>(SdlSubSystemType::SDL_mixer)] == SdlStatus::Error)
    {
        logService->error("SDL_mixer could not initialize! SDL_mixer Error: \n");
        logService->error(Mix_GetError());
    }
}

SdlStatus SdlSubsystems::subsystemStatus(SdlSubSystemType type) const
{
    return isInitialized ? status[static_cast<int>(type)] : SdlStatus::Initialized;
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
