#include "transform.h"
#include "gtest/gtest.h"

using namespace Mino;

class TransformTests : public ::testing::Test
{
public:
    Transform tr = Transform{};
};

TEST_F(TransformTests, CanCreateWithoutParent) {}

TEST_F(TransformTests, CanAddChild)
{
	auto child = tr.addChild(); 
}

TEST_F(TransformTests, RemovingChildReducesTheChildcount)
{

    ASSERT_EQ(tr.childCount(), 0);

    auto child = tr.addChild();

    ASSERT_EQ(tr.childCount(), 1);

    tr.removeChild(child);

    ASSERT_EQ(tr.childCount(), 0);
}
