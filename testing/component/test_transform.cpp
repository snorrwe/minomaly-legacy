#include "transform.h"
#include "gtest/gtest.h"

using namespace Mino;

class TransformTests : public ::testing::Test
{
public:
};

TEST_F(TransformTests, CanCreateWithoutParent)
{
    auto tr = Transform::create();
    ASSERT_TRUE(tr);
}

TEST_F(TransformTests, CanAddChild)
{
    auto tr = Transform::create();

    auto child = tr->addChild();

    ASSERT_EQ(child.getParent(), tr.get());
}

TEST_F(TransformTests, CanAddExitingChildToNewParent)
{
    auto parent1 = Transform::create();
    auto parent2 = Transform::create();

    auto child = parent1->addChild();

    ASSERT_EQ(child.getParent(), parent1.get());
    ASSERT_NE(child.getParent(), parent2.get());

    child = parent2->addChild(child);

    ASSERT_EQ(child.getParent(), parent2.get());
    ASSERT_NE(child.getParent(), parent1.get());

    ASSERT_EQ(parent2->childCount(), 1);
    ASSERT_EQ(parent1->childCount(), 0);
}

TEST_F(TransformTests, RemovingChildReducesTheChildcount)
{
    auto tr = Transform::create();

    ASSERT_EQ(tr->childCount(), 0);

    auto child = tr->addChild();

    ASSERT_EQ(tr->childCount(), 1);

    tr->removeChild(child);

    ASSERT_EQ(tr->childCount(), 0);
}
