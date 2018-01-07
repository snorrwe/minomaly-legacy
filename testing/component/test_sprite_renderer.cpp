#include "render_system.h"
#include "sprite_renderer.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace Mino;

class MockRenderSystem : public IRenderSystem
{
public:
    MOCK_METHOD3(render, void(Texture const&, SDL_Rect*, SDL_Rect*));
    MOCK_METHOD4(render, void(Texture const&, SDL_Rect*, SDL_Rect*, RotationData const&));
    MOCK_METHOD3(loadTexture, std::shared_ptr<Texture>(std::string const&, bool, Color const*));
    MOCK_METHOD1(setViewport, void(SDL_Rect* viewport));
    MOCK_METHOD0(clear, void());
    MOCK_METHOD0(update, void());
    MOCK_METHOD0(getRaw, SDL_Renderer*());
};

class SpriteRendererTests : public ::testing::Test
{
public:
};

TEST_F(SpriteRendererTests, CanBeCreatedByRenderSystem)
{
    auto system = std::make_shared<MockRenderSystem>();

    auto result = system->createRenderer<SpriteRenderComponent>();

    ASSERT_TRUE(result);
}
