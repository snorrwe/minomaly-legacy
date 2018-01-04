#pragma once
#include "SDL.h"
#include "SDL_image.h"
#undef main
#include <iostream>
#include <memory>

class SdlSubsystems
{
public:
    static std::shared_ptr<SdlSubsystems> initialize();

    SdlSubsystems() = default;
    SdlSubsystems(SdlSubsystems const&) = delete;
    SdlSubsystems(SdlSubsystems&&) = delete;
    ~SdlSubsystems();

    SdlSubsystems& operator=(SdlSubsystems const&) = delete;
    SdlSubsystems& operator=(SdlSubsystems&&) = delete;

private:
    static bool isInitialized;
    static SdlSubsystems* instance;
};
