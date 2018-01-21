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

template <> std::unique_ptr<FakeComponent> Component::create<FakeComponent>()
{
    GameObjectTests::called = true;
    return std::make_unique<FakeComponent>();
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

TEST_F(GameObjectTests, CanSelfDisableFromComponentUpdate)
{
    class SelfDisablerComponent : public Component
    {
    public:
        virtual void update()
        {
            updates++;
            disable();
        }

        size_t updates = 0;
    };

    auto go = GameObject();

    auto first = go.addComponent<SelfDisablerComponent>();
    auto second = go.addComponent<SelfDisablerComponent>();
    auto third = go.addComponent<SelfDisablerComponent>();

    ASSERT_EQ(first->updates, 0);
    ASSERT_EQ(second->updates, 0);
    ASSERT_EQ(third->updates, 0);

    go.update();

    ASSERT_EQ(first->updates, 1);
    ASSERT_EQ(second->updates, 1);
    ASSERT_EQ(third->updates, 1);

    go.update();

    ASSERT_EQ(first->updates, 1);
    ASSERT_EQ(second->updates, 1);
    ASSERT_EQ(third->updates, 1);
}

TEST_F(GameObjectTests, CanToggleAnotherComponentFromUpdate)
{
    class FakeComponent1 : public Component
    {
    public:
        virtual void update()
        {
            updates++;
            disable();
        }

        size_t updates = 0;
    };

    class FakeComponent2 : public Component
    {
    public:
        virtual void start()
        {
            fake1 = gameObject->getComponent<FakeComponent1>();
            if (!fake1) FAIL();
        }

        virtual void update()
        {
            if (!updates++)
                fake1->disable();
            else
                fake1->enable();
        }

        size_t updates = 0;
        FakeComponent1* fake1 = nullptr;
    };

    auto go = GameObject();

    auto first = go.addComponent<FakeComponent1>();
    auto second = go.addComponent<FakeComponent2>();

    for (int i = 0; i < 500; ++i)
    {
        go.update();

        ASSERT_EQ(first->updates, i / 2 + 1);
        ASSERT_EQ(second->updates, i + 1);
    }
}
