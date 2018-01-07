#include "mouse.h"

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
    subs = Subscriptions{input->onKeyDown([&](auto const& e) {
        if (e.key.keysym.sym == SDLK_ESCAPE) engine->stop();
    })};

    mouse = createGameObject();
    auto mousePic = renderer->loadTexture("assets/mouse/mouse.png");
    mouse->addComponent<SpriteRenderComponent>()->setTexture(mousePic);
}

void Program::update() {}
