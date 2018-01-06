#include "fox.h"

using namespace Mino;

Program::Program(std::shared_ptr<Core> core) : Scene(core)
{
    renderer = engine->getRenderer();
    input = engine->getInput();
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
    font = Font::load("assets/fox/fox.ttf", 28);
    textures = MediaContainer{
        Texture::loadText("Type stuff!", *renderer, *font),
    };
    subs = Subscriptions{input->onKeyDown([&](auto const& e) {
        if (e.key.keysym.sym == SDLK_ESCAPE)
        {
            engine->stop();
        }
        else
        {
            textures.push_back(
                Texture::loadText(SDL_GetKeyName(e.key.keysym.sym), *renderer, *font));
        }
    })};
}

void Program::update()
{
    int posX = 0, posY = 0;
    for (auto i = textures.begin(); i != textures.end(); ++i)
    {
        auto& texture = *i;
        texture->render({posX, posY});
        posX += texture->getWidth();
        if (posX > SCREEN_WIDTH)
        {
            posX = 0;
            posY += texture->getHeight();
        }
    }
}
