#pragma once
#include "SDL_mixer.h"
#include "asset.h"
#include "audio_system.h"
#include "p_managed_audio.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace Mino
{
class IAudioSystem;

class Audio : public Asset
{
public:
    static std::shared_ptr<Audio> loadWAV(std::string const& fname);

    Audio(std::unique_ptr<Private::ManagedAudio>&& audio) : audio(std::move(audio)) {}
    Audio(Audio const&) = default;
    Audio(Audio&&) = default;
    ~Audio() {}

    Audio& operator=(Audio const&) = default;
    Audio& operator=(Audio&&) = default;

    operator Mix_Chunk*() const { return *audio; }

private:
    std::unique_ptr<Private::ManagedAudio> audio;
};

class Music : public Asset
{
public:
    static std::shared_ptr<Music> loadMusic(std::string const& fname);

    Music(std::unique_ptr<Private::ManagedMusic>&& music) : music(std::move(music))
    {
        volume = Mix_VolumeMusic(-1);
    }
    Music(Music const&) = default;
    Music(Music&&) = default;
    ~Music() {}

    Music& operator=(Music const&) = default;
    Music& operator=(Music&&) = default;

    operator Mix_Music*() const { return *music; }

    int getVolume() { return volume; }
    void setVolume(int value)
    {
        assert(value >= 0);
        volume = value;
        Mix_VolumeMusic(value);
    }

private:
    std::unique_ptr<Private::ManagedMusic> music;
    int volume = -1;
};

} // namespace Mino
