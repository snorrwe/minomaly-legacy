#include "runner.h"

using namespace Mino;

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
        // std::cout << "Child: " << gameObject->getTransform()->position() << " "
        //          << gameObject->getTransform()->absolute().position << std::endl;
    }
};

void Program::start()
{
    renderer = engine->getRenderer();
    input = engine->getInput();
    time = engine->getTime();
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

    auto currentChild = egg;
    for (auto i = 0; i < 5; ++i)
    {
        auto childEgg = createGameObject<Child>();
        currentChild->addChild(*childEgg);
        childEgg->addComponent<SpriteRenderComponent>()->setTexture(eggPic);
        childEgg->getTransform()->setPosition({10 * i, 30});
        currentChild = childEgg;
    }
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
