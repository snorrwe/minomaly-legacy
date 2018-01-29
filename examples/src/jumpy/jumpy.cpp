#include "jumpy.h"

using namespace Mino;

Program::~Program()
{
    for (auto i = subs.begin(); i != subs.end(); ++i)
    {
        i->unsubscribe();
    }
}

void Program::start()
{
    renderer = engine->getRenderer();
    input = engine->getInput();
    subs = Subscriptions{input->onKeyDown([&](auto const& e) {
        if (e.key.keysym.sym == SDLK_ESCAPE) engine->stop();
    })};
}

void Program::update() {}
