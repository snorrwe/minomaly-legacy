#include "mouse.h"

using namespace Mino;

Program::Program(std::shared_ptr<EngineCore> core) : Scene(core)
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

    images = {renderer->loadTexture("assets/mouse/mouse.png")};
    for (int i = 0; i < 50; ++i)
    {
        mice.push_back(createGameObject<SpriteRenderComponent>());
        mice.back()->getComponent<SpriteRenderComponent>()->setTexture(images[0]);
    }
}

void Program::update()
{
    auto mousePos = input->mouseScreenPosition();
    auto width = 0.5 * images[0]->getWidth();
    auto height = 0.5 * images[0]->getHeight();
    auto x = (double)mousePos.x() - width;
    auto y = (double)mousePos.y() - height;
    (*mice.begin())->getTransform()->setPosition(x, y);

    auto next = mice.rbegin() + 1;
    auto current = mice.rbegin();
    for (; current != mice.rend() - 1; ++current, ++next)
    {
        (*current)->getTransform()->setPosition((*next)->getTransform()->getPosition());
    }
}
