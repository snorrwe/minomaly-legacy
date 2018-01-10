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
    mouse = createGameObject<SpriteRenderComponent, PhysicsComponent>();
    physics = mouse->getComponent<PhysicsComponent>();
    mouse->getComponent<SpriteRenderComponent>()->setTexture(images[0]);
    mouse->getTransform()->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void Program::update()
{
    auto mousePos = input->mouseScreenPosition();
    auto width = 0.5 * images[0]->getWidth();
    auto height = 0.5 * images[0]->getHeight();
    auto x = (double)mousePos.x() - width;
    auto y = (double)mousePos.y() - height;

    auto target = Vector2<double>(x, y);

    physics->setVelocity(target - mouse->getTransform()->getPosition());
}
