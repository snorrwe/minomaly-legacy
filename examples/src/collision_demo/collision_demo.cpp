#include "collision_demo.h"

using namespace Mino;

Program::Program(std::shared_ptr<EngineCore> core) : Scene(core) { input = engine->getInput(); }

Program::~Program()
{
    for (auto i = subs.begin(); i != subs.end(); ++i)
    {
        i->unsubscribe();
    }
}

void Program::start()
{
    subs = Subscriptions{input->onKeyDown([&](auto const& e) {
        if (e.key.keysym.sym == SDLK_ESCAPE) engine->stop();
    })};
}

void Program::update() {}
