#include "game_object.h"
#include "transform.h"
#include "gtest/gtest.h"

using namespace Mino;

class TestGameObjectTransformIntegration : public ::testing::Test
{
public:
    void SetUp() { rootTransform = Transform::getRoot(); }

    void TearDown() { rootTransform = nullptr; }

    Transform::TransformRef rootTransform;
};
