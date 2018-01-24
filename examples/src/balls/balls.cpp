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
    std::srand(std::time(nullptr));
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(
        1, SCREEN_WIDTH > SCREEN_HEIGHT ? SCREEN_WIDTH
                                        : SCREEN_HEIGHT); // distribution in range [1, 6]

    for (int i = 0; i < 300; ++i)
    {
        auto x = dist6(rng) % (SCREEN_WIDTH - 50) + 50;
        auto y = dist6(rng) % (SCREEN_HEIGHT - 50) + 50;

        auto egg = createGameObject<SpriteRenderComponent, BoxColliderComponent, PhysicsComponent,
                                    BallComponent>({x, y});
        egg->getComponent<SpriteRenderComponent>()->setTexture(eggPic);
    }

    auto leftWall = createGameObject<BoxColliderComponent>({0, 0});
    auto leftWallCollider = leftWall->getComponent<BoxColliderComponent>();
    leftWallCollider->set(3000, SCREEN_HEIGHT, {-3000, 0});

    auto rightWall = createGameObject<BoxColliderComponent>({SCREEN_WIDTH, 0});
    auto rightWallCollider = rightWall->getComponent<BoxColliderComponent>();
    rightWallCollider->set(3000, SCREEN_HEIGHT, {0, 0});

    auto topWall = createGameObject<BoxColliderComponent>({0, SCREEN_HEIGHT});
    auto topWallCollider = topWall->getComponent<BoxColliderComponent>();
    topWallCollider->set(SCREEN_WIDTH, 3000, {0, 0});

    auto botWall = createGameObject<BoxColliderComponent>({0, 0});
    auto botWallCollider = botWall->getComponent<BoxColliderComponent>();
    botWallCollider->set(SCREEN_WIDTH, 3000, {0, -3000});
}

void Program::update() {}
