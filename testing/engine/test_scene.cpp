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

class MockAudioSystem : public IAudioSystem
{
public:
    MOCK_METHOD3(playChannel, void(Audio const&, int channel, int loops));
    MOCK_METHOD0(musicPlaying, bool());
    MOCK_METHOD0(musicPaused, bool());
    MOCK_METHOD2(playMusic, void(Music const&, int loops));
    MOCK_METHOD0(resumeMusic, void());
    MOCK_METHOD0(pauseMusic, void());
    MOCK_METHOD0(stopMusic, void());
};

class EngineCoreMock : public Core
{
public:
    EngineCoreMock()
        : Core(std::make_shared<MockSubsystems>(), std::make_shared<MockInput>(),
               std::move(std::make_unique<MockWindow>()), std::make_shared<MockRenderer>(),
               std::make_shared<MockAudioSystem>())
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
    SceneTests() : scene((std::static_pointer_cast<Core>(std::make_shared<EngineCoreMock>()))) {}

    void SetUp() {}

    FakeScene scene;
};

class FakeComponent1 : public Component
{
};

class FakeComponent2 : public Component
{
};

class FakeComponent3 : public Component
{
};

class FakeComponent4 : public Component
{
};

TEST_F(SceneTests, CanCreateGameObjectCorrectly)
{
    auto go = scene.createGameObject<>();
    ASSERT_EQ(go->getScene(), &scene);
    ASSERT_EQ(go->getTransform()->getParent(), scene.getRootTransform().get());
}

TEST_F(SceneTests, CanCreateGameObjectWithComponents)
{
    auto gameObject =
        scene.createGameObject<FakeComponent1, FakeComponent2, FakeComponent3, FakeComponent4>();

    ASSERT_TRUE(gameObject->getComponent<FakeComponent1>());
    ASSERT_TRUE(gameObject->getComponent<FakeComponent2>());
    ASSERT_TRUE(gameObject->getComponent<FakeComponent3>());
    ASSERT_TRUE(gameObject->getComponent<FakeComponent4>());
}
