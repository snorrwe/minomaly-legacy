#pragma once
#include "SDL_mixer.h"
#include "audio_system.h"
#include "p_managed_audio.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace Mino
{

class IAudioSystem;

class Audio
{
public:
    static std::shared_ptr<Audio> loadWAV(std::string const& fname);

    Audio(std::shared_ptr<Private::ManagedAudio> audio) : audio(audio) {}
    Audio(Audio const&) = default;
    Audio(Audio&&) = default;
    ~Audio() {}

    Audio& operator=(Audio const&) = default;
    Audio& operator=(Audio&&) = default;

    operator Mix_Chunk*() const { return *audio; }

private:
    std::shared_ptr<Private::ManagedAudio> audio;
};

class Music
{
public:
    static std::shared_ptr<Music> loadMusic(std::string const& fname);

    Music(std::shared_ptr<Private::ManagedMusic> music) : music(music) {}
    Music(Music const&) = default;
    Music(Music&&) = default;
    ~Music() {}

    Music& operator=(Music const&) = default;
    Music& operator=(Music&&) = default;

    operator Mix_Music*() const { return *music; }

private:
    std::shared_ptr<Private::ManagedMusic> music;
};

} // namespace Mino
