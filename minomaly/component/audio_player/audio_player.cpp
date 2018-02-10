#include "audio_player.h"

using namespace Mino;

void AudioPlayerComponent::start()
{
    assert(minomaly);
    audioSystem = minomaly->getAudio();
    assert(audioSystem);
}

void AudioPlayerComponent::play(int channel, int loops)
{
    assert(audioSystem);
    assert(audio);
    audioSystem->playChannel(*audio, channel, loops);
}

void AudioPlayerComponent::play(Audio& aud, int channel, int loops)
{
    audio = &aud;
    play(channel, loops);
}
