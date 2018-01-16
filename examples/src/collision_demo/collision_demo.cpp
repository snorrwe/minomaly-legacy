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
    auto renderer = engine->getRenderer();
    subs = Subscriptions{};
    gameObjects = {
        createGameObject<SpriteRenderComponent, BoxColliderComponent, PhysicsComponent>(),
        createGameObject<SpriteRenderComponent, BoxColliderComponent>(
            Vector2<double>{(SCREEN_WIDTH - 50) * 0.5, (SCREEN_HEIGHT - 150) * 0.5}),
    };

    const auto egg = "assets/collision_demo/egg.png";
    gameObjects[0]->getComponent<SpriteRenderComponent>()->setTexture(renderer->loadTexture(egg));
    gameObjects[0]->getComponent<BoxColliderComponent>()->set(30, 30);
    gameObjects[0]->getTransform()->setPosition(15, 30);
    eggPhysics = gameObjects[0]->getComponent<PhysicsComponent>();

    const auto bar = "assets/collision_demo/bar.png";
    gameObjects[1]->getComponent<SpriteRenderComponent>()->setTexture(renderer->loadTexture(bar));
    gameObjects[1]->getComponent<BoxColliderComponent>()->set(30, 120);
}

void Program::update()
{
    auto mousePos = input->mouseScreenPosition();
    auto width = 25;
    auto height = 25;
    auto x = (double)mousePos.x() - width;
    auto y = (double)mousePos.y() - height;

    auto target = Vector2<double>(x, y);

    eggPhysics->setVelocity(target - gameObjects[0]->getTransform()->getPosition());
}
