#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#undef main
#include <iostream>
#include <memory>

namespace Mino
{

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

private:
    static bool isInitialized;
    static SdlSubsystems* instance;
};

} // namespace Mino
