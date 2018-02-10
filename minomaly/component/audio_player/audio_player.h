#pragma once
#include "audio.h"
#include "audio_system.h"
#include "component.h"
#include "core.h"
#include <cassert>
#include <memory>

namespace Mino
{
class AudioPlayerComponent : public Component
{
public:
    virtual void start();

    void setAudio(Audio* value) { audio = value; }
    Audio* getAudio() { return audio; }

    void play(int channel = -1, int loops = 0);
    void play(Audio& audio, int channel = -1, int loops = 0);

protected:
    IAudioSystem* audioSystem = nullptr;
    Audio* audio;
};

} // namespace Mino
