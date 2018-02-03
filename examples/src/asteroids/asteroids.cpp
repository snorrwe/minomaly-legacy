#include "asteroids.h"

using namespace Mino;

void Program::start()
{
    auto rng = std::mt19937{};
    rng.seed(std::random_device()());
    auto dist6 = std::uniform_int_distribution<std::mt19937::result_type>(0, WORLD_WIDTH);
    auto getRandom = [&]() { return dist6(rng); };

    for (int i = 0; i < 500; ++i)
    {
        auto x = getRandom();
        auto y = getRandom();

        auto asteroid = createGameObject<SpriteRendererComponent, SpriteAnimatorComponent,
                                         BoxColliderComponent, Rigidbody, AsteroidComponent>();
        asteroid->getTransform()->setPosition({x, y});

        auto vx = getRandom() / 1000.f;
        auto vy = getRandom() / 1000.f;

        auto velocity = Vector2<float>(vx, vy);
        asteroid->getComponent<AsteroidComponent>()->setup(WORLD_WIDTH, velocity);
    }

    getEngineCore()->getPhysicsSystem()->setWorldBox({{0, 0}, 0, 0});
}
