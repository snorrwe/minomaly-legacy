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
    input = engine->getInput();
    time = Services::get<ITimeService>().get();
    subs = Subscriptions{};
    auto bar = createGameObject<SpriteRendererComponent, BoxColliderComponent>(
        {(SCREEN_WIDTH - 50) * 0.5, 0.0});

    images.push_back(engine->getAssets()->loadTexture("assets/runner/bar.png"));
    bar->getComponent<SpriteRendererComponent>()->setTexture(images.back().get());
    auto barCollider = bar->getComponent<BoxColliderComponent>();
    barCollider->set(30, 120);
    barCollider->setLayers(0x1 | 0x2);

    auto egg = createGameObject<SpriteRendererComponent, SpriteAnimatorComponent,
                                BoxColliderComponent, Rigidbody, EggComponent>({0, 0});
    egg->addChild(*getMainCamera());
    getMainCamera()->getTransform()->position() =
        getMainCamera()->getTransform()->position() +
        Vector2<float>{SCREEN_WIDTH * -0.5f, SCREEN_HEIGHT * -0.5f};
    auto eggPic =
        engine->getAssets()->loadSpriteSheet("assets/runner/egg.png", {{0, 0, 30, 30}})[0];
    images.push_back(eggPic);
    auto eggEgg = egg->getComponent<EggComponent>();
    eggEgg->input = input;
    eggEgg->bottom = 0;
    egg->getTransform()->setPosition({50, 0.0});

    auto currentChild = egg;
    for (auto i = 0; i < 5; ++i)
    {
        auto childEgg = createGameObject<Child>();
        currentChild->addChild(*childEgg);
        childEgg->addComponent<SpriteRendererComponent>()->setTexture(eggPic.get());
        childEgg->getTransform()->setPosition({10 * i, 30});
        currentChild = childEgg;
    }

    // getMainCamera()->getTransform()->setScale({0.5f, 0.5f});
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
    if (input->isDown(SDLK_e))
    {
        getMainCamera()->getTransform()->rotation().angle += time->deltaTime() * 2;
    }
    if (input->isDown(SDLK_q))
    {
        getMainCamera()->getTransform()->rotation().angle -= time->deltaTime() * 2;
    }
    velocity = velocity * time->deltaTime();

    auto& cameraPos = getMainCamera()->getTransform()->position();
    cameraPos = cameraPos + velocity;
}
