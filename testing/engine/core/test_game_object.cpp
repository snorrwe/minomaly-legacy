#include "component.h"
#include "game_object.h"
#include "gtest/gtest.h"
#include <memory>

using namespace Mino;

class FakeComponent : public Component
{
public:
    virtual void update() { updates++; }
    void mySuperAwesomeFunction() {}

    size_t updates = 0;
};

class GameObjectTests : public ::testing::Test
{
public:
    static bool called;

    void SetUp() { called = false; }
};

bool GameObjectTests::called = false;

template <> static std::shared_ptr<FakeComponent> Component::create<FakeComponent>()
{
    GameObjectTests::called = true;
    return std::make_shared<FakeComponent>();
}

TEST_F(GameObjectTests, CanAddComponentWithNoError)
{
    auto go = GameObject();
    auto result = go.addComponent<FakeComponent>();
}

TEST_F(GameObjectTests, CanFindComponentByType)
{
    auto go = GameObject();
    auto actual = go.addComponent<FakeComponent>();
    auto expected = go.getComponent<FakeComponent>();

    if (!expected)
    {
        FAIL();
    }

    expected->mySuperAwesomeFunction();
    ASSERT_EQ(actual, expected);
}

TEST_F(GameObjectTests, ReturnsNullPtrIfNotFound)
{

    class FakeComponent2 : public Component
    {
    };

    auto go = GameObject();
    auto actual = go.addComponent<FakeComponent>();
    auto expected = go.getComponent<FakeComponent2>();

    if (expected)
    {
        FAIL();
    }
}

TEST_F(GameObjectTests, CanOverwriteCreate)
{
    auto go = GameObject();
    auto actual = go.addComponent<FakeComponent>();

    ASSERT_TRUE(called);
}

TEST_F(GameObjectTests, UpdatesComponents)
{
    auto go = GameObject();

    auto first = go.addComponent<FakeComponent>();
    auto second = go.addComponent<FakeComponent>();

    ASSERT_EQ(first->updates, 0);
    ASSERT_EQ(second->updates, 0);

    go.update();

    ASSERT_EQ(first->updates, 1);
    ASSERT_EQ(second->updates, 1);
}

TEST_F(GameObjectTests, DoesNotUpdateDisabledComponents)
{
    auto go = GameObject();

    auto first = go.addComponent<FakeComponent>();
    auto second = go.addComponent<FakeComponent>();

    ASSERT_EQ(first->updates, 0);
    ASSERT_EQ(second->updates, 0);

    go.update();

    ASSERT_EQ(first->updates, 1);
    ASSERT_EQ(second->updates, 1);

    first->disable();
    go.update();

    ASSERT_EQ(first->updates, 1);
    ASSERT_EQ(second->updates, 2);
}

TEST_F(GameObjectTests, CanReenableComponent)
{
    auto go = GameObject();

    auto first = go.addComponent<FakeComponent>();
    auto second = go.addComponent<FakeComponent>();

    ASSERT_EQ(first->updates, 0);
    ASSERT_EQ(second->updates, 0);

    go.update();

    ASSERT_EQ(first->updates, 1);
    ASSERT_EQ(second->updates, 1);

    first->disable();
    go.update();

    ASSERT_EQ(first->updates, 1);
    ASSERT_EQ(second->updates, 2);

    first->enable();
    go.update();

    ASSERT_EQ(first->updates, 2);
    ASSERT_EQ(second->updates, 3);
}
