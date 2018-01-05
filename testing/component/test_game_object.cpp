#include "component.h"
#include "game_object.h"
#include "gtest/gtest.h"
#include <memory>

using namespace Mino;

class FakeComponent : public Component
{
public:
    void mySuperAwesomeFunction() {}
};

TEST(GameObject, CanAddComponentWithNoError)
{
    auto go = GameObject();
    auto result = go.addComponent<FakeComponent>();
}

TEST(GameObject, CanFindComponentByType)
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

TEST(GameObject, ReturnsNullPtrIfNotFound)
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
