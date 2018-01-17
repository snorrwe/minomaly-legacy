#include "component.h"
#include "scene.h"
#include "sdl_subsystems.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>
#include <vector>

using namespace Mino;

class MockEngineCore : public IEngineCore
{
public:
    MOCK_METHOD0(run, void());
    MOCK_METHOD0(stop, void());
    MOCK_CONST_METHOD0(getWindow, std::shared_ptr<IWindowSystem>());
    MOCK_CONST_METHOD0(getInput, std::shared_ptr<IInputSystem>());
    MOCK_CONST_METHOD0(getRenderer, std::shared_ptr<IRenderSystem>());
    MOCK_CONST_METHOD0(getAudio, std::shared_ptr<IAudioSystem>());
    MOCK_CONST_METHOD0(getScene, std::shared_ptr<Scene>());
    MOCK_CONST_METHOD0(getPhysicsSystem, std::shared_ptr<IPhysicsSystem>());
    MOCK_CONST_METHOD0(getTime, std::shared_ptr<ITimeService>());
    MOCK_METHOD1(setScene, void(std::shared_ptr<Scene> scene));
    MOCK_CONST_METHOD1(subsystemStatus, SdlStatus(SdlSubSystemType));
    MOCK_CONST_METHOD1(subsystemStatus,
                       std::vector<SdlStatus>(std::vector<SdlSubSystemType> const&));
    MOCK_METHOD1(setTargetFps, void(double f));
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
