#include "game_object.h"
#include "transform.h"
#include "gtest/gtest.h"
#include <memory>

using namespace Mino;

class TestGameObjectTransformIntegration : public ::testing::Test
{
public:
    void SetUp()
    {
        rootTransform = Transform::getRoot();
        gameObject = std::make_unique<GameObject>(rootTransform->addChild());
    }

    void TearDown() { rootTransform = nullptr; }

    Transform::TransformRef rootTransform;
    std::unique_ptr<GameObject> gameObject;
};

TEST_F(TestGameObjectTransformIntegration, CanAddChild)
{
    auto rootTr = gameObject->getTransform();
    auto child = GameObject(rootTransform->addChild());
    gameObject->addChild(child);
    auto childTr = child.getTransform();
    childTr->setPosition({1.0, -1.0});

    rootTransform->updateChildren();

    EXPECT_FLOAT_EQ(childTr->absolute().position.x(), 1.0f);
    EXPECT_FLOAT_EQ(childTr->absolute().position.y(), -1.0f);

    rootTr->setPosition({1.0, 0.0});
    rootTransform->updateChildren();

    EXPECT_FLOAT_EQ(childTr->absolute().position.x(), 2.0f);
    EXPECT_FLOAT_EQ(childTr->absolute().position.y(), -1.0f);
}

TEST_F(TestGameObjectTransformIntegration, CanAddMultipleLayersOfChildren)
{
    auto child1 = GameObject(rootTransform->addChild());
    auto child2 = GameObject(rootTransform->addChild());
    auto child3 = GameObject(rootTransform->addChild());

    gameObject->addChild(child1);
    child1.addChild(child2);
    child2.addChild(child3);

    EXPECT_EQ(child1.getParent(), gameObject.get());
    EXPECT_EQ(child2.getParent(), &child1);
    EXPECT_EQ(child3.getParent(), &child2);
}

TEST_F(TestGameObjectTransformIntegration, AddingChildInvalidatesExistingReferences)
{ /* TODO: fix: adding child GameObjects doesnt invalidate references */
    auto rootTr = gameObject->getTransform();
    auto child = GameObject(rootTransform->addChild());
    auto childTr = child.getTransform();
    childTr->setPosition({1.0, -1.0});
    gameObject->addChild(child);

    rootTransform->updateChildren();

    EXPECT_FLOAT_EQ(childTr->absolute().position.x(), 1.0f);
    EXPECT_FLOAT_EQ(childTr->absolute().position.y(), -1.0f);

    rootTr->setPosition({1.0, 0.0});
    rootTransform->updateChildren();

    EXPECT_NE(childTr->absolute().position.x(), 2.0f); // Fix here
    EXPECT_FLOAT_EQ(childTr->absolute().position.y(), -1.0f);
}
