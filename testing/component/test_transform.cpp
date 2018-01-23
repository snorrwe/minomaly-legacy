#include "transform.h"
#include "gtest/gtest.h"

using namespace Mino;

class TransformTests : public ::testing::Test
{
public:
    void SetUp() { tr = Transform::getRoot(); }

    void TearDown() { tr = nullptr; }

    Transform::TransformRef tr;
};

TEST_F(TransformTests, CanCreateWithoutParent) {}

TEST_F(TransformTests, CanAddChild) { auto child = tr->addChild(); }

TEST_F(TransformTests, RemovingChildReducesTheChildcount)
{

    ASSERT_EQ(tr->childCount(), 0);

    auto child = tr->addChild();

    ASSERT_EQ(tr->childCount(), 1);

    tr->removeChild(child);

    ASSERT_EQ(tr->childCount(), 0);
}

TEST_F(TransformTests, ChildrenPositionsAreRelativeToParent)
{
    tr->setPosition({0, 0});

    auto child1 = tr->addChild();
    auto child2 = child1->addChild();
    child1->setPosition({1, 0});
    child2->setPosition({2, 0});

    tr->updateAsRoot();

    ASSERT_EQ(child1->position().x(), 1);
    ASSERT_EQ(child1->absolute().position.x(), 1);
    ASSERT_EQ(child2->position().x(), 2);
    ASSERT_EQ(child2->absolute().position.x(), 3);

    tr->setPosition({1, 0});
    tr->updateAsRoot();

    ASSERT_EQ(child1->position().x(), 1);
    ASSERT_EQ(child1->absolute().position.x(), 2);
    ASSERT_EQ(child2->position().x(), 2);
    ASSERT_EQ(child2->absolute().position.x(), 4);
}
