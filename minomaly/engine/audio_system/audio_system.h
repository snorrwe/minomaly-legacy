#pragma once
#include "audio.h"
#include <memory>

namespace Mino
{
class Audio;
class Music;

class IAudioSystem
{
public:
    const int LoopForever = -1;

    virtual ~IAudioSystem() {}

    virtual void playChannel(Audio const&, int channel = -1, int loops = 0) = 0;

    virtual bool musicPlaying() = 0;
    virtual bool musicPaused() = 0;

    virtual void playMusic(Music const&, int loops = -1) = 0;
    virtual void resumeMusic() = 0;
    virtual void pauseMusic() = 0;
    virtual void stopMusic() = 0;
};

class MuteAudioSystem : public IAudioSystem
{
public:
    virtual ~MuteAudioSystem() {}

    virtual void playChannel(Audio const&, int channel = -1, int loops = 0) {}
    virtual bool musicPlaying() { return false; }
    virtual bool musicPaused() { return false; }
    virtual void playMusic(Music const&, int loops = 1) {}
    virtual void resumeMusic() {}
    virtual void pauseMusic() {}
    virtual void stopMusic() {}
};

class AudioSystem : public IAudioSystem
{
public:
    static std::shared_ptr<IAudioSystem> create();

    virtual ~AudioSystem() {}

    virtual void playChannel(Audio const&, int channel = -1, int loops = 0);

    virtual bool musicPlaying();
    virtual bool musicPaused();

    virtual void playMusic(Music const&, int loops = 1);
    virtual void resumeMusic();
    virtual void pauseMusic();
    virtual void stopMusic();
};

} // namespace Mino
