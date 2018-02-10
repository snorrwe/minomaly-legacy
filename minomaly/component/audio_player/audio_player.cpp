#include "audio_player.h"

using namespace Mino;

void AudioPlayerComponent::start()
{
    audioSystem = minomaly->getAudio();
    assert(audioSystem);
}
