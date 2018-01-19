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

class Child : public Component
{
public:
    virtual void update()
    {
        std::cout << "Child: " << gameObject->getTransform()->position() << " "
                  << gameObject->getTransform()->absolute().position << std::endl;
    }
};

void Program::start()
{
    auto renderer = engine->getRenderer();
    subs = Subscriptions{};
    auto bar = createGameObject<SpriteRenderComponent, BoxColliderComponent>(
        {(SCREEN_WIDTH - 50) * 0.5, 0.0});

    bar->getComponent<SpriteRenderComponent>()->setTexture(
        renderer->loadTexture("assets/runner/bar.png"));
    auto barCollider = bar->getComponent<BoxColliderComponent>();
    barCollider->set(30, 120);
    barCollider->setLayers(0x1 | 0x2);

    auto egg = createGameObject<SpriteRenderComponent, BoxColliderComponent, PhysicsComponent,
                                EggComponent>({0, 0});
    auto eggPic = renderer->loadTexture("assets/runner/egg.png");
    auto eggEgg = egg->getComponent<EggComponent>();
    eggEgg->input = input;
    eggEgg->bottom = 0;
    egg->getComponent<SpriteRenderComponent>()->setTexture(eggPic);
    egg->getTransform()->setPosition({50, 0.0});

    auto childEgg = createGameObject<Child>();
    egg->addChild(*childEgg);
    childEgg->addComponent<SpriteRenderComponent>()->setTexture(eggPic);
    childEgg->getTransform()->setPosition({0, 30});
}

void Program::update()
{
    auto velocity = Mino::Vector2<float>{0.0f, 0.0f};
    auto sv = 200.0f;
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

    auto& cameraPos = getMainCamera()->getTransform()->position();
    cameraPos = cameraPos + velocity;
}
