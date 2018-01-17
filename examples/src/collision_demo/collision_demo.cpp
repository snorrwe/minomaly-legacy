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
    auto barGo = createGameObject<SpriteRenderComponent, BoxColliderComponent>(
        {(SCREEN_WIDTH - 50) * 0.5, (SCREEN_HEIGHT - 150) * 0.5});

    const auto bar = "assets/collision_demo/bar.png";
    barGo->getComponent<SpriteRenderComponent>()->setTexture(renderer->loadTexture(bar));
    barGo->getComponent<BoxColliderComponent>()->set(30, 120);

    const auto egg = "assets/collision_demo/egg.png";
    eggPic = renderer->loadTexture(egg);
    for (int i = 0; i < 5; ++i)
    {
        addEgg();
    }
}

void Program::addEgg()
{
    eggs.push_back(createGameObject<SpriteRenderComponent, BoxColliderComponent, PhysicsComponent>(
        {(double)eggs.size(), (double)eggs.size()}));
    eggs.back()->getComponent<SpriteRenderComponent>()->setTexture(eggPic);
    auto eggCollider = eggs.back()->getComponent<BoxColliderComponent>();
    eggCollider->set(30, 30);
    auto eggPhysics = eggs.back()->getComponent<PhysicsComponent>();
    eggPhysics->addCollider(eggCollider);
}

void Program::update()
{
    auto mousePos = input->mouseScreenPosition();
    const auto width = 15.0;
    const auto height = 15.0;
    auto x = (double)mousePos.x() - width;
    auto y = (double)mousePos.y() - height;

    for (auto i = eggs.begin(); i != eggs.end(); ++i)
    {
        auto& egg = *i;
        auto velocity = Vector2<double>(x, y) - egg->getTransform()->getPosition();
        velocity = velocity.normalized() * 150.0;
        egg->getComponent<PhysicsComponent>()->setVelocity(velocity);
    }
}
