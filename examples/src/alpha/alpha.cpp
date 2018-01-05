#include "alpha.h"

using namespace Mino;

Program::Program(std::shared_ptr<Core> core) : ILogicSystem(core)
{
    renderer = engine->getRenderer();
    camera = Camera::create(renderer);
    const auto padding = 15;
    camera->updateViewport(padding, padding, SCREEN_WIDTH - padding * 2,
                           SCREEN_HEIGHT - padding * 2);
    input = engine->getInput();
    subs = Subscriptions{input->onKeyDown([&](auto const& e) {
        if (e.key.keysym.sym == SDLK_ESCAPE) engine->stop();
    })};
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
    loadMedia();
    images[0]->setBlendMode(SDL_BLENDMODE_BLEND);
}

void Program::update()
{
    if (!(wait += 2))
    {
        if (input->isDown(SDLK_UP))
        {
            if (alpha + 32 > 255)
            {
                alpha = 255;
            }
            else
            {
                alpha += 32;
            }
            images[1]->setAlpha(alpha);
        }
        else if (input->isDown(SDLK_DOWN))
        {
            if (alpha - 32 < 0)
            {
                alpha = 0;
            }
            else
            {
                alpha -= 32;
            }
            images[1]->setAlpha(alpha);
        }
    }
    images[0]->render({0, 0});
    images[1]->render({0, 0});
}

void Program::loadMedia()
{
    const auto colorKey = Color(0, 0xFF, 0xFF);
    images = MediaContainer{loadTexture("assets/alpha/fadein.png", true, &colorKey),
                            loadTexture("assets/alpha/fadeout.png", true, &colorKey)};
}

std::shared_ptr<Texture> Program::loadTexture(std::string const& name, bool flag,
                                              Color const* color)
{
    auto result = renderer->loadTexture(name, flag, color);
    return result;
}
