#include "game_object.h"
#include "render_system.h"
#include "renderer.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>
#include <vector>

using namespace Mino;
using ::testing::_;

class MockRenderComponent : public RenderComponent
{
public:
    MOCK_METHOD1(render, void(Transform::TransformRef camera));
    MOCK_METHOD0(start, void());
    MOCK_METHOD0(update, void());

    void setRenderSystem(std::weak_ptr<IRenderSystem> r) { renderSystem = r; }
};

class RenderSystemTests : public ::testing::Test
{
public:
    void SetUp()
    {
        renderer = std::make_shared<RenderSystem>(nullptr);
        for (int i = 0; i < 5; ++i)
        {
            mockRenderers.push_back(renderer->createRenderer<MockRenderComponent>());
        }
    }

    void TearDown() { renderer = nullptr; }

protected:
    std::shared_ptr<RenderSystem> renderer;
    std::vector<std::unique_ptr<MockRenderComponent>> mockRenderers = {};
};

TEST_F(RenderSystemTests, CanCreate) {}

TEST_F(RenderSystemTests, RendersRenderComponentsOnUpdate)
{
    for (auto& i : mockRenderers)
    {
        EXPECT_CALL(*i, render(_)).Times(5);
    }

    for (int i = 0; i < 5; ++i)
    {
        renderer->update();
    }
}

TEST_F(RenderSystemTests, OnlyRendersEnabledRenderers)
{
    EXPECT_CALL(*mockRenderers[0], render(_)).Times(5);
    EXPECT_CALL(*mockRenderers[2], render(_)).Times(5);
    EXPECT_CALL(*mockRenderers[3], render(_)).Times(5);
    EXPECT_CALL(*mockRenderers[4], render(_)).Times(5);

    renderer->disableRenderer(mockRenderers[1].get());
    EXPECT_CALL(*mockRenderers[1], render(_)).Times(0);

    for (int i = 0; i < 5; ++i)
    {
        renderer->update();
    }
}
