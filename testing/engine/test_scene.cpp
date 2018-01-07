#include "component.h"
#include "scene.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

using namespace Mino;

class MockSubsystems : public SdlSubsystems
{
public:
};

class MockWindow : public IWindow
{
public:
    MOCK_METHOD4(fillRect, void(const SDL_Rect*, uint8_t, uint8_t, uint8_t));
    MOCK_METHOD3(blitSurface, void(SDL_Surface*, const SDL_Rect*, SDL_Rect*));
    MOCK_METHOD3(blitScaled, void(SDL_Surface*, const SDL_Rect*, SDL_Rect*));
};

class MockInput : public IInputSystem
{
public:
    MOCK_METHOD0(update, void());
    MOCK_METHOD1(isDown, bool(SDL_Keycode const&));
    MOCK_METHOD1(isUp, bool(SDL_Keycode const&));
    MOCK_METHOD1(onQuit, ISubscription(std::function<void(uint8_t)>));
    MOCK_METHOD1(onKeyDown, ISubscription(std::function<void(SDL_Event)>));
    MOCK_METHOD1(onKeyUp, ISubscription(std::function<void(SDL_Event)>));
    MOCK_METHOD0(mouseScreenPosition, Vector2<int>());
};

class MockRenderer : public IRenderSystem
{
public:
    MOCK_METHOD3(render, void(Texture const&, SDL_Rect*, SDL_Rect*));
    MOCK_METHOD4(render, void(Texture const&, SDL_Rect*, SDL_Rect*, RotationData const&));
    MOCK_METHOD3(loadTexture, std::shared_ptr<Texture>(std::string const&, bool, Color const*));
    MOCK_METHOD1(setViewport, void(SDL_Rect* viewport));
    MOCK_METHOD0(getRaw, SDL_Renderer*());
    MOCK_METHOD0(clear, void());
    MOCK_METHOD0(update, void());
};

class EngineCoreMock : public Core
{
public:
    EngineCoreMock()
        : Core(std::make_shared<MockSubsystems>(), std::make_shared<MockInput>(),
               std::move(std::make_unique<MockWindow>()), std::make_shared<MockRenderer>())
    {
    }
};

class FakeScene : public Scene
{
public:
    FakeScene(std::shared_ptr<Core> engine) : Scene(engine) {}
    ~FakeScene() {}
    void update() {}
};

class SceneTests : public ::testing::Test
{
public:
    void SetUp() {}
    FakeScene scene = FakeScene(std::static_pointer_cast<Core>(std::make_shared<EngineCoreMock>()));
};

TEST_F(SceneTests, CanCreateGameObjectCorrectly)
{
    auto go = scene.createGameObject();
    ASSERT_EQ(go->getScene(), &scene);
    ASSERT_EQ(go->getTransform()->getParent(), scene.getRootTransform().get());
}
