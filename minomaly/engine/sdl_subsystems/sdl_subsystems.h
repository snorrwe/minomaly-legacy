#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "log_service.h"
#undef main
#include <array>
#include <iostream>
#include <memory>
#include <vector>

namespace Mino
{
enum class SdlSubSystemType
{
    SDL = 0,
    SDL_image = 1,
    SDL_ttf = 2,
    SDL_mixer = 3,
    COUNT = 4
};

enum class SdlStatus
{
    Error = 0,
    Initialized = 1,
    UnInitialized = 2,
};

class SdlSubsystems
{
public:
    static std::unique_ptr<SdlSubsystems> initialize(std::shared_ptr<ILogService>);

    SdlSubsystems(std::shared_ptr<ILogService>);
    SdlSubsystems(SdlSubsystems const&) = delete;
    SdlSubsystems(SdlSubsystems&&) = delete;
    virtual ~SdlSubsystems();

    SdlSubsystems& operator=(SdlSubsystems const&) = delete;
    SdlSubsystems& operator=(SdlSubsystems&&) = delete;

    SdlStatus subsystemStatus(SdlSubSystemType) const;
    std::vector<SdlStatus> subsystemStatus(std::vector<SdlSubSystemType> const&) const;

private:
    static bool isInitialized;
    static std::array<SdlStatus, static_cast<int>(SdlSubSystemType::COUNT)> status;

    static void Init_SDL(std::shared_ptr<ILogService> logService);
    static void Init_SDL_image(std::shared_ptr<ILogService> logService);
    static void Init_SDL_ttf(std::shared_ptr<ILogService> logService);
    static void Init_SDL_mixer(std::shared_ptr<ILogService> logService);

    std::shared_ptr<ILogService> logService;
};

} // namespace Mino
