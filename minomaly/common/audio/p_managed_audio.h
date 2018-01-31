#pragma once
#include "SDL_mixer.h"

namespace Mino::Private
{

struct ManagedAudio
{
    ManagedAudio(Mix_Chunk* chunk) : chunk(chunk) {}
    ManagedAudio(ManagedAudio const&) = delete;
    ManagedAudio(ManagedAudio&&) = delete;
    ~ManagedAudio() { Mix_FreeChunk(chunk); }

    ManagedAudio& operator=(ManagedAudio const&) = delete;
    ManagedAudio& operator=(ManagedAudio&&) = delete;

    operator Mix_Chunk*() const { return chunk; }

    Mix_Chunk* chunk = nullptr;
};

struct ManagedMusic
{
    ManagedMusic(Mix_Music* music) : music(music) {}
    ManagedMusic(ManagedMusic const&) = delete;
    ManagedMusic(ManagedMusic&&) = delete;
    ~ManagedMusic() { Mix_FreeMusic(music); }

    ManagedMusic& operator=(ManagedMusic const&) = delete;
    ManagedMusic& operator=(ManagedMusic&&) = delete;

    operator Mix_Music*() const { return music; }

    Mix_Music* music = nullptr;
};

} // namespace Mino::Private
