#include "component.h"
#include "scene.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

using namespace Mino;

class MockEngineCore : public IEngineCore
{
public:
    MOCK_METHOD0(run, void());
    MOCK_METHOD0(stop, void());
    MOCK_METHOD0(getWindow, std::shared_ptr<IWindowSystem>());
    MOCK_METHOD0(getInput, std::shared_ptr<IInputSystem>());
    MOCK_METHOD0(getRenderer, std::shared_ptr<IRenderSystem>());
    MOCK_METHOD0(getAudio, std::shared_ptr<IAudioSystem>());
    MOCK_METHOD0(getLogic, std::shared_ptr<Scene>());
    MOCK_METHOD1(setLogic, void(std::shared_ptr<Scene> logic));
};

class FakeScene : public Scene
{
public:
    FakeScene(std::shared_ptr<IEngineCore> engine) : Scene(engine) {}
    ~FakeScene() {}
    void update() {}
};

class SceneTests : public ::testing::Test
{
public:
    SceneTests() : scene(std::make_shared<MockEngineCore>()) {}

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
