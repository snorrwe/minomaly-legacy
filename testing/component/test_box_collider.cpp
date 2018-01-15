#include "box_collider.h"
#include "game_object.h"
#include "transform.h"
#include "gtest/gtest.h"
#include <memory>

using namespace Mino;

class BoxColliderTests : public ::testing::Test
{
public:
    void SetUp()
    {
        go1 = GameObject(rootTransforms.enable());
        go2 = GameObject(rootTransforms.enable());
        collider1 = go1.addComponent<BoxCollider>();
        collider2 = go2.addComponent<BoxCollider>();
    }

    GameObject go1;
    std::shared_ptr<BoxCollider> collider1;

    GameObject go2;
    std::shared_ptr<BoxCollider> collider2;

    Transform::ChildrenContainer rootTransforms;
};

TEST_F(BoxColliderTests, CanCreate) {}

TEST_F(BoxColliderTests, ReturnsFalseOnNotIntersecting)
{
    go1.getTransform()->setPosition(5, 5);
    go2.getTransform()->setPosition(-5, -5);

    go1.getTransform()->flip();
    go2.getTransform()->flip();

    collider1->set(3, 3);
    collider2->set(3, 3);

    go1.update();
    go2.update();

    ASSERT_FALSE(collider1->overlapping(*collider2));
    ASSERT_FALSE(collider2->overlapping(*collider1));
}

TEST_F(BoxColliderTests, ReturnsTrueOnIntersecting)
{
    go1.getTransform()->setPosition(5, 5);
    go2.getTransform()->setPosition(-5, -5);

    go1.getTransform()->flip();
    go2.getTransform()->flip();

    collider1->set(11, 11);
    collider2->set(11, 11);

    go1.update();
    go2.update();

    ASSERT_TRUE(collider1->overlapping(*collider2));
    ASSERT_TRUE(collider2->overlapping(*collider1));
}
