#include "balls.h"

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
    auto eggPic = renderer->loadTexture("assets/balls/egg.png");

    // const size_t SCREEN_WIDTH = 640;
    // const size_t SCREEN_HEIGHT = 480;
    std::srand(std::time(nullptr));
    for (int i = 0; i < 20; ++i)
    {
        auto x = std::rand() % 60 * 10 + 50;
        auto y = std::rand() % 45 * 10 + 50;

        auto egg = createGameObject<SpriteRenderComponent, BoxColliderComponent, PhysicsComponent,
                                    BallComponent>({x, y});
        egg->getComponent<SpriteRenderComponent>()->setTexture(eggPic);
    }

    auto leftWall = createGameObject<BoxColliderComponent>({0, 0});
    auto leftWallCollider = leftWall->getComponent<BoxColliderComponent>();
    leftWallCollider->set(3000, 480, {-3000, 0});

    auto rightWall = createGameObject<BoxColliderComponent>({640, 0});
    auto rightWallCollider = rightWall->getComponent<BoxColliderComponent>();
    rightWallCollider->set(3000, 480, {0, 0});

    auto topWall = createGameObject<BoxColliderComponent>({0, 480});
    auto topWallCollider = topWall->getComponent<BoxColliderComponent>();
    topWallCollider->set(640, 3000, {0, 0});

    auto botWall = createGameObject<BoxColliderComponent>({0, 0});
    auto botWallCollider = botWall->getComponent<BoxColliderComponent>();
    botWallCollider->set(640, 3000, {0, -3000});
}

void Program::update() {}
