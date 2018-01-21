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

    ASSERT_FLOAT_EQ(childTr->absolute().position.x(), 1.0f);
    ASSERT_FLOAT_EQ(childTr->absolute().position.y(), -1.0f);

    rootTr->setPosition({1.0, 0.0});
    rootTransform->updateChildren();

    ASSERT_FLOAT_EQ(childTr->absolute().position.x(), 2.0f);
    ASSERT_FLOAT_EQ(childTr->absolute().position.y(), -1.0f);
}
/*
// TODO

TEST_F(TestGameObjectTransformIntegration, AddingChildDoesntInvalidateExistingReferences)
{
    auto rootTr = gameObject->getTransform();
    auto child = GameObject(rootTransform->addChild());
    auto childTr = child.getTransform();
    childTr->setPosition({1.0, -1.0});
    gameObject->addChild(child);

    rootTransform->updateChildren();

    ASSERT_FLOAT_EQ(childTr->absolute().position.x(), 1.0f);
    ASSERT_FLOAT_EQ(childTr->absolute().position.y(), -1.0f);

    rootTr->setPosition({1.0, 0.0});
    rootTransform->updateChildren();

    ASSERT_FLOAT_EQ(childTr->absolute().position.x(), 2.0f);
    ASSERT_FLOAT_EQ(childTr->absolute().position.y(), -1.0f);
}
*/