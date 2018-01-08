#include "audio_play.h"

using namespace Mino;

Program::Program(std::shared_ptr<Core> core) : Scene(core)
{
    renderer = engine->getRenderer();
    input = engine->getInput();
    audioSystem = engine->getAudioSystem();
}

Program::~Program()
{
    for (auto i = subs.begin(); i != subs.end(); ++i)
    {
        i->unsubscribe();
    }
}

void Program::start()
{
    subs = Subscriptions{
        input->onKeyDown([&](auto const& e) {
            switch (e.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                engine->stop();
                break;
            case SDLK_1:
                audioSystem->playChannel(*audio[0]);
                break;
            case SDLK_2:
                audioSystem->playChannel(*audio[1]);
                break;

            case SDLK_3:
                audioSystem->playChannel(*audio[2]);
                break;

            case SDLK_4:
                audioSystem->playChannel(*audio[3]);
                break;

            case SDLK_9:
            {
                if (!audioSystem->musicPlaying())
                {
                    audioSystem->playMusic(*music[0], AudioSystem::LoopForever);
                }
                else
                {
                    if (audioSystem->musicPaused())
                    {
                        audioSystem->resumeMusic();
                    }
                    else
                    {
                        audioSystem->pauseMusic();
                    }
                }
                break;
            }
            case SDLK_0:
                audioSystem->stopMusic();
                break;
            }
        }),
    };

    images = MediaContainer{renderer->loadTexture("assets/audio_play/prompt.png")};

    background = createGameObject<SpriteRenderComponent>();
    background->getComponent<SpriteRenderComponent>()->setTexture(images[0]);

    audio = AudioContainer{
        Audio::loadWAV("assets/audio_play/high.wav"),
        Audio::loadWAV("assets/audio_play/medium.wav"),
        Audio::loadWAV("assets/audio_play/low.wav"),
        Audio::loadWAV("assets/audio_play/scratch.wav"),
    };

    music = MusicContainer{Music::loadMusic("assets/audio_play/beat.wav")};
}

void Program::update() {}
