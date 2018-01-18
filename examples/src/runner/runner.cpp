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
        {(SCREEN_WIDTH - 50) * 0.5, 0.0});

    const auto bar = "assets/runner/bar.png";
    barGo->getComponent<SpriteRenderComponent>()->setTexture(renderer->loadTexture(bar));
    auto barCollider = barGo->getComponent<BoxColliderComponent>();
    barCollider->set(30, 120);
    barCollider->setLayers(0x1 | 0x2);

    auto eggGo = createGameObject<SpriteRenderComponent, BoxColliderComponent, PhysicsComponent,
                                  EggComponent>({0, 0});
    const auto egg = "assets/runner/egg.png";
    auto eggPic = renderer->loadTexture(egg);
    auto eggEgg = eggGo->getComponent<EggComponent>();
    eggEgg->input = input;
    eggEgg->bottom = 0;
    eggGo->getComponent<SpriteRenderComponent>()->setTexture(eggPic);
    eggGo->getTransform()->setPosition(50, 0.0);
}

void Program::update()
{
    Mino::Vector2<float> velocity = {0, 0};
    float sv = 200;
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
    velocity = velocity * time->deltaTime();

    auto cameraTransform = renderer->getMainCamera()->getTransform();
    cameraTransform->setPosition(cameraTransform->getPosition() + velocity);
}
