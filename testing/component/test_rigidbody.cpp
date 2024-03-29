#include "game_object.h"
#include "rigidbody.h"
#include "services.h"
#include "time_service.h"
#include "transform.h"
#include "gtest/gtest.h"
#include <chrono>
#include <memory>

using namespace Mino;

class FakeTime : public TimeService
{
public:
    void setDtime(Milli dtime)
    {
        TimePoint now{};
        update(now);
        auto then = now + dtime;
        update(then);
    }
};

class TestRigidbody : public ::testing::Test
{
public:
    void SetUp()
    {
        fakeTime = std::make_shared<FakeTime>();
        Services::overrideService<ITimeService>(fakeTime);
        gameObject = std::make_unique<GameObject>(transforms.enable());
        physics = gameObject->addComponent<Rigidbody>();
        physics->start();
    }

    void TearDown() { gameObject.reset(); }

protected:
    std::unique_ptr<GameObject> gameObject;
    Rigidbody* physics;
    std::shared_ptr<FakeTime> fakeTime;
    Transform::ChildrenContainer transforms;
};

TEST_F(TestRigidbody, CanCreate) {}

TEST_F(TestRigidbody, MovesGameObjectOnUpdate)
{
    auto lastPosition = gameObject->getTransform()->getPosition();
    physics->setVelocity({1, 1});
    for (int i = 1; i < 50; ++i)
    {
        auto milliseconds = FakeTime::Milli{10 * i};
        fakeTime->setDtime(milliseconds);
        physics->update();
        auto currentPosition = gameObject->getTransform()->getPosition();
        auto delta = currentPosition - lastPosition;
        lastPosition = currentPosition;

        ASSERT_NEAR(delta.x(), milliseconds.count() / 1000, 0.001);
        ASSERT_NEAR(delta.y(), milliseconds.count() / 1000, 0.001);
    }
}
