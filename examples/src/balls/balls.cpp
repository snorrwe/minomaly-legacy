#include "balls.h"

using namespace Mino;

Program::~Program()
{
    for (auto i = subs.begin(); i != subs.end(); ++i)
    {
        i->unsubscribe();
    }
}

void Program::start()
{
    renderer = engine->getRenderer();
    input = engine->getInput();

    const auto OFFSET = 300;

    engine->getPhysicsSystem()->setWorldBox({{SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f},
                                             SCREEN_WIDTH + (OFFSET * 2),
                                             SCREEN_HEIGHT + (OFFSET * 2)});

    auto eggPic = renderer->loadTexture("assets/balls/egg.png");
    std::srand(std::time(nullptr));
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(
        1, SCREEN_WIDTH > SCREEN_HEIGHT ? SCREEN_WIDTH : SCREEN_HEIGHT);
    for (int i = 0; i < 1500; ++i)
    {
        auto x = dist6(rng) % (SCREEN_WIDTH - 50) + 50.0f;
        auto y = dist6(rng) % (SCREEN_HEIGHT - 50) + 50.0f;

        auto egg = createGameObject<SpriteRenderComponent, BoxColliderComponent, PhysicsComponent,
                                    BallComponent>({x, y});
        egg->getComponent<SpriteRenderComponent>()->setTexture(eggPic);
    }

    auto leftWall = createGameObject<BoxColliderComponent>({0, 0});
    auto leftWallCollider = leftWall->getComponent<BoxColliderComponent>();
    leftWallCollider->set(OFFSET, SCREEN_HEIGHT, {-OFFSET, 0});

    auto rightWall = createGameObject<BoxColliderComponent>({(float)SCREEN_WIDTH, 0});
    auto rightWallCollider = rightWall->getComponent<BoxColliderComponent>();
    rightWallCollider->set(OFFSET, SCREEN_HEIGHT, {0, 0});

    auto topWall = createGameObject<BoxColliderComponent>({0, (float)SCREEN_HEIGHT});
    auto topWallCollider = topWall->getComponent<BoxColliderComponent>();
    topWallCollider->set(SCREEN_WIDTH, OFFSET, {0, 0});

    auto botWall = createGameObject<BoxColliderComponent>({0, 0});
    auto botWallCollider = botWall->getComponent<BoxColliderComponent>();
    botWallCollider->set(SCREEN_WIDTH, OFFSET, {0, -OFFSET});
}

void Program::update() {}
