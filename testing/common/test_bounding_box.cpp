#include "bounding_box.h"
#include "gtest/gtest.h"

using namespace Mino;

class TestBoundingBox : public ::testing::Test
{
public:
};

TEST_F(TestBoundingBox, TestIntersectsContaining)
{
    BoundingBox box1{{0, 0}, 5};
    BoundingBox box2{{0, 0}, 2};

    ASSERT_TRUE(box1.intersects(box2));
    ASSERT_TRUE(box2.intersects(box1));
}

TEST_F(TestBoundingBox, TestIntersectsItself)
{
    BoundingBox box{{0, 0}, 1};

    ASSERT_TRUE(box.intersects(box));
}

TEST_F(TestBoundingBox, TestRecognisesNotIntersecing)
{
    BoundingBox box1{{5, 5}, 4};
    BoundingBox box2{{5, -5}, 4};
    BoundingBox box3{{-5, 5}, 4};
    BoundingBox box4{{-5, -5}, 4};

    ASSERT_FALSE(box1.intersects(box2));
    ASSERT_FALSE(box1.intersects(box3));
    ASSERT_FALSE(box1.intersects(box4));

    ASSERT_FALSE(box2.intersects(box1));
    ASSERT_FALSE(box2.intersects(box3));
    ASSERT_FALSE(box2.intersects(box4));

    ASSERT_FALSE(box3.intersects(box1));
    ASSERT_FALSE(box3.intersects(box2));
    ASSERT_FALSE(box3.intersects(box4));

    ASSERT_FALSE(box4.intersects(box1));
    ASSERT_FALSE(box4.intersects(box3));
    ASSERT_FALSE(box4.intersects(box2));
}

TEST_F(TestBoundingBox, TestRecognisesIntersecing)
{
    BoundingBox box1{{5, 5}, 6};
    BoundingBox box2{{5, -5}, 6};
    BoundingBox box3{{-5, 5}, 6};
    BoundingBox box4{{-5, -5}, 6};

    ASSERT_TRUE(box1.intersects(box2));
    ASSERT_TRUE(box1.intersects(box3));
    ASSERT_TRUE(box1.intersects(box4));

    ASSERT_TRUE(box2.intersects(box1));
    ASSERT_TRUE(box2.intersects(box3));
    ASSERT_TRUE(box2.intersects(box4));

    ASSERT_TRUE(box3.intersects(box1));
    ASSERT_TRUE(box3.intersects(box2));
    ASSERT_TRUE(box3.intersects(box4));

    ASSERT_TRUE(box4.intersects(box1));
    ASSERT_TRUE(box4.intersects(box3));
    ASSERT_TRUE(box4.intersects(box2));
}
