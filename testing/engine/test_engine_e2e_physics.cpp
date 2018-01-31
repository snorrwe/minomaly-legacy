#include "mino.h"
#include "gtest/gtest.h"
#include <memory>
#include <random>

using namespace Mino;

const auto BOX_WIDTH = 500;
const auto BOX_HEIGHT = 400;
const auto BALL_RADIUS = 30;
const auto PADDING = 150;
const auto OFFSET = 300;

constexpr auto MAX_BALLS_FITTING_IN_BOX =
    (BOX_WIDTH - PADDING) * (BOX_HEIGHT - PADDING) / (BALL_RADIUS * BALL_RADIUS);

class BallComponent : public Mino::Component
{
public:
    virtual void start()
    {
        auto eggCollider = gameObject->getComponent<Mino::BoxColliderComponent>();
        eggCollider->set(BALL_RADIUS, BALL_RADIUS, {0.0f, 0.0f});
        eggCollider->onCollision().subscribe([&](auto const& collisionData) {
            auto v =
                collisionData.second.getPositionDelta() - collisionData.first.getPositionDelta();
            body->setVelocity(v.normalized() * velocity);
        });

        body = gameObject->getComponent<Mino::Rigidbody>();
        body->addCollider(*eggCollider);
        body->setMaterial({1.0f});

        auto vx = std::rand() % 100 - 50.0f;
        auto vy = std::rand() % 100 - 50.0f;
        body->setVelocity(Mino::Vector2<float>{vx, vy}.normalized() * velocity);
    }

    Mino::Rigidbody* body;
    const float velocity = 250.0f;
};

class BallsTestApp : public Mino::Application
{
public:
    virtual void start();
    virtual void update();

    std::vector<Mino::GameObject*> balls = {};
    ITimeService* time;
    float elapsedMs = 0;
    size_t updates = 0;

    const float LIMIT = 0.5f;
};

void BallsTestApp::update()
{
    elapsedMs += time->deltaTime();
    ++updates;
    if (elapsedMs >= LIMIT)
    {
        getEngineCore()->stop();
    }
}

void BallsTestApp::start()
{
    time = Services::get<ITimeService>().get();

    engine->getPhysicsSystem()->setWorldBox({{BOX_WIDTH * 0.5f, BOX_HEIGHT * 0.5f},
                                             BOX_WIDTH + (OFFSET * 2),
                                             BOX_HEIGHT + (OFFSET * 2)});

    std::srand(std::time(nullptr));
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(
        1, BOX_WIDTH > BOX_HEIGHT ? BOX_WIDTH : BOX_HEIGHT);

    auto maxBalls = MAX_BALLS_FITTING_IN_BOX - (int)(MAX_BALLS_FITTING_IN_BOX * 0.1f);
    for (int i = 0; i < maxBalls; ++i)
    {
        auto x = dist6(rng) % (BOX_WIDTH - PADDING) + PADDING;
        auto y = dist6(rng) % (BOX_HEIGHT - PADDING) + PADDING;

        auto ball = createGameObject<BoxColliderComponent, Rigidbody, BallComponent>({x, y});
        balls.push_back(ball);
    }

    auto leftWall = createGameObject<BoxColliderComponent>({0, 0});
    auto leftWallCollider = leftWall->getComponent<BoxColliderComponent>();
    leftWallCollider->set(OFFSET, BOX_HEIGHT, {-OFFSET, 0});

    auto rightWall = createGameObject<BoxColliderComponent>({(float)BOX_WIDTH, 0});
    auto rightWallCollider = rightWall->getComponent<BoxColliderComponent>();
    rightWallCollider->set(OFFSET, BOX_HEIGHT, {0, 0});

    auto topWall = createGameObject<BoxColliderComponent>({0, (float)BOX_HEIGHT});
    auto topWallCollider = topWall->getComponent<BoxColliderComponent>();
    topWallCollider->set(BOX_WIDTH, OFFSET, {0, 0});

    auto botWall = createGameObject<BoxColliderComponent>({0, 0});
    auto botWallCollider = botWall->getComponent<BoxColliderComponent>();
    botWallCollider->set(BOX_WIDTH, OFFSET, {0, -OFFSET});
}

class TestEngineE2EPhysics : public ::testing::Test
{
public:
    void SetUp() { engine = EngineCore::create<BallsTestApp>("TestEngineE2EPhysics", 0, 0); }

    void TearDown() { engine.reset(); }

    std::shared_ptr<EngineCore> engine = nullptr;
};

TEST_F(TestEngineE2EPhysics, CanCreate) { EXPECT_TRUE(engine); }

TEST_F(TestEngineE2EPhysics, BallsDoNotEscapeTheBox)
{
    auto app = static_cast<BallsTestApp*>(engine->getApplication());
    app->elapsedMs = 0.0f;
    engine->run();

    for (auto& go : app->balls)
    {
        auto pos = go->getTransform()->absolute().position;

        EXPECT_GE(pos.x(), -OFFSET);
        EXPECT_LE(pos.x(), BOX_WIDTH + OFFSET);
        EXPECT_GE(pos.y(), -OFFSET);
        EXPECT_LE(pos.y(), BOX_HEIGHT + OFFSET);
    }
}
