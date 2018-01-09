#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
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
    Initialized = 1
};

class SdlSubsystems
{
public:
    static std::shared_ptr<SdlSubsystems> initialize();

    SdlSubsystems() = default;
    SdlSubsystems(SdlSubsystems const&) = delete;
    SdlSubsystems(SdlSubsystems&&) = delete;
    virtual ~SdlSubsystems();

    SdlSubsystems& operator=(SdlSubsystems const&) = delete;
    SdlSubsystems& operator=(SdlSubsystems&&) = delete;

    SdlStatus subsystemStatus(SdlSubSystemType) const;
    std::vector<SdlStatus> subsystemStatus(std::vector<SdlSubSystemType> const&) const;

private:
    static bool isInitialized;
    static SdlSubsystems* instance;
    static std::array<SdlStatus, static_cast<int>(SdlSubSystemType::COUNT)> status;

    static void Init_SDL();
    static void Init_SDL_image();
    static void Init_SDL_ttf();
    static void Init_SDL_mixer();
};

} // namespace Mino
