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
    void SetUp() { renderer = std::make_shared<RenderSystem>(nullptr); }

    std::shared_ptr<MockRenderComponent> createMockRenderer()
    {
        return renderer->createRenderer<MockRenderComponent>();
    }

protected:
    std::shared_ptr<RenderSystem> renderer;
};

TEST_F(RenderSystemTests, CanCreateFromCtor) {}

TEST_F(RenderSystemTests, CanCreateMockRenderers)
{
    auto renderers = std::vector<std::shared_ptr<MockRenderComponent>>{
        createMockRenderer(), createMockRenderer(), createMockRenderer(),
        createMockRenderer(), createMockRenderer(),
    };

    ASSERT_EQ(renderers.size(), 5);
}

TEST_F(RenderSystemTests, RendersRenderComponentsOnUpdate)
{
    auto renderers = std::vector<std::shared_ptr<MockRenderComponent>>{
        createMockRenderer(), createMockRenderer(), createMockRenderer(),
        createMockRenderer(), createMockRenderer(),
    };

    for (auto i = renderers.begin(); i != renderers.end(); ++i)
    {
        EXPECT_CALL(**i, render(_)).Times(5);
    }

    for (int i = 0; i < 5; ++i)
    {
        renderer->update();
    }
}

TEST_F(RenderSystemTests, OnlyRendersEnabledRenderers)
{
    auto mockRenderers = std::vector<std::shared_ptr<MockRenderComponent>>{
        createMockRenderer(), createMockRenderer(), createMockRenderer(),
        createMockRenderer(), createMockRenderer(), createMockRenderer(),
    };

    EXPECT_CALL(*mockRenderers[0], render(_)).Times(5);
    EXPECT_CALL(*mockRenderers[2], render(_)).Times(5);
    EXPECT_CALL(*mockRenderers[3], render(_)).Times(5);
    EXPECT_CALL(*mockRenderers[4], render(_)).Times(5);
    EXPECT_CALL(*mockRenderers[5], render(_)).Times(5);

    renderer->disableRenderer(*mockRenderers[1]);
    EXPECT_CALL(*mockRenderers[1], render(_)).Times(0);

    for (int i = 0; i < 5; ++i)
    {
        renderer->update();
    }
}
