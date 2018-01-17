#include "runner.h"

using namespace Mino;

Program::Program(std::shared_ptr<EngineCore> core) : Scene(core)
{
    renderer = engine->getRenderer();
    input = engine->getInput();
    time = engine->getTime();
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
    auto renderer = engine->getRenderer();
    subs = Subscriptions{};
    auto barGo = createGameObject<SpriteRenderComponent, BoxColliderComponent>(
        {(SCREEN_WIDTH - 50) * 0.5, (SCREEN_HEIGHT - 150) * 0.5});

    const auto bar = "assets/collision_demo/bar.png";
    barGo->getComponent<SpriteRenderComponent>()->setTexture(renderer->loadTexture(bar));
    barGo->getComponent<BoxColliderComponent>()->set(30, 120);

    auto eggGo = createGameObject<SpriteRenderComponent, BoxColliderComponent, PhysicsComponent,
                                  EggComponent>({0, 0});
    const auto egg = "assets/collision_demo/egg.png";
    auto eggPic = renderer->loadTexture(egg);
    eggGo->getComponent<SpriteRenderComponent>()->setTexture(eggPic);
    auto eggCollider = eggGo->getComponent<BoxColliderComponent>();
    eggCollider->set(30, 30);
    auto eggPhysics = eggGo->getComponent<PhysicsComponent>();
    eggPhysics->addCollider(eggCollider);
    eggGo->getComponent<EggComponent>()->input = input;
}

void Program::update()
{
    Mino::Vector2<double> velocity = {0, 0};
    double sv = 20;
    if (input->isDown(SDLK_a))
    {
        velocity = {velocity.x() - sv, velocity.y()};
    }
    if (input->isDown(SDLK_d))
    {
        velocity = {velocity.x() + sv, velocity.y()};
    }
    if (input->isDown(SDLK_w))
    {
        velocity = {velocity.x(), velocity.y() + sv};
    }
    if (input->isDown(SDLK_s))
    {
        velocity = {velocity.x(), velocity.y() - sv};
    }
    velocity = velocity * (1 / time->deltaTime());

    auto cameraTransform = renderer->getMainCamera()->getTransform();
    cameraTransform->setPosition(cameraTransform->getPosition() + velocity);
}
