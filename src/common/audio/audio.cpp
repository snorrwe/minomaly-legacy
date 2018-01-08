#include "audio.h"

using namespace Mino;
using namespace Mino::Private;

std::shared_ptr<Audio> Audio::loadWAV(std::string const& fname)
{
    auto result = Mix_LoadWAV(fname.c_str());
    if (result == nullptr)
    {
        std::cout << "Failed to load audio " << fname << " SDL_mixer Error:\n"
                  << Mix_GetError() << std::endl;
        throw std::runtime_error("Failed to load audio");
    }
    return std::make_shared<Audio>(std::make_shared<ManagedAudio>(result));
}

std::shared_ptr<Music> Music::loadMusic(std::string const& fname)
{
    auto result = Mix_LoadMUS(fname.c_str());
    if (result == nullptr)
    {
        std::cout << "Failed to load music " << fname << " SDL_mixer Error:\n"
                  << Mix_GetError() << std::endl;
        throw std::runtime_error("Failed to load audio");
    }
    return std::make_shared<Music>(std::make_shared<ManagedMusic>(result));
}
