#include "fox.h"

using namespace Mino;

Program::Program(std::shared_ptr<Core> core) : ILogicSystem(core)
{
    renderer = engine->getRenderer();
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
    font = Font::load("assets/fox/fox.ttf", 28);
    textures = MediaContainer{
        Texture::loadText("The quick brown fox jumps over the lazy dog!.?", *renderer, *font),
        Texture::loadText("Fukin gud", *renderer, *font),
        Texture::loadText(":*", *renderer, *font),
    };
}

void Program::update()
{
    int pos = 0;
    for (auto i = textures.begin(); i != textures.end(); ++i, pos += 50)
    {
        (*i)->render({0, pos});
    }
}
