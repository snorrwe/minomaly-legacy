#include "audio_system.h"

using namespace Mino;

void AudioSystem::playChannel(Audio const& audio, int channel, int loops)
{
    Mix_PlayChannel(channel, audio, loops);
}

bool AudioSystem::musicPlaying() { return Mix_PlayingMusic(); }
bool AudioSystem::musicPaused() { return Mix_PausedMusic(); }

void AudioSystem::playMusic(Music const& music, int loops) { Mix_PlayMusic(music, loops); }

void AudioSystem::resumeMusic() { Mix_ResumeMusic(); }
void AudioSystem::pauseMusic() { Mix_PauseMusic(); }
void AudioSystem::stopMusic() { Mix_HaltMusic(); }

std::shared_ptr<IAudioSystem> AudioSystem::create() { return std::make_shared<AudioSystem>(); }
