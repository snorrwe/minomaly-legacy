#include "application.h"
#include "component.h"
#include "sdl_subsystems.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>
#include <vector>

using namespace Mino;
using ::testing::Return;

class MockEngineCore : public IEngineCore
{
public:
    MOCK_METHOD0(run, void());
    MOCK_METHOD0(stop, void());
    MOCK_CONST_METHOD0(getWindow, IWindowSystem*());
    MOCK_CONST_METHOD0(getInput, IInputSystem*());
    MOCK_CONST_METHOD0(getRenderer, IRenderSystem*());
    MOCK_CONST_METHOD0(getAudio, IAudioSystem*());
    MOCK_CONST_METHOD0(getApplication, Application*());
    MOCK_CONST_METHOD0(getPhysicsSystem, IPhysicsSystem*());
    MOCK_CONST_METHOD0(getAssets, IAssetSystem*());
    MOCK_CONST_METHOD1(subsystemStatus, SdlStatus(SdlSubSystemType));
    MOCK_CONST_METHOD1(subsystemStatus,
                       std::vector<SdlStatus>(std::vector<SdlSubSystemType> const&));
    MOCK_METHOD1(setTargetFps, void(float f));
};

class FakeApp : public Application
{
public:
    using Application::engine;

    ~FakeApp() {}
    void update() {}
};

class TestApplication : public ::testing::Test
{
public:
    void SetUp()
    {
        mockEngineCore = std::make_unique<MockEngineCore>();
        application.setEngineCore(mockEngineCore.get());
        ON_CALL(*mockEngineCore, getApplication()).WillByDefault(Return(&application));
    }

    FakeApp application = {};
    std::unique_ptr<MockEngineCore> mockEngineCore = nullptr;
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

TEST_F(TestApplication, CanCreateGameObjectCorrectly)
{
    auto go = application.createGameObject<>();
    ASSERT_EQ(go->getApplication(), &application);
}

TEST_F(TestApplication, CanCreateGameObjectWithComponents)
{
    auto gameObject
        = application
              .createGameObject<FakeComponent1, FakeComponent2, FakeComponent3, FakeComponent4>();

    ASSERT_TRUE(gameObject->getComponent<FakeComponent1>());
    ASSERT_TRUE(gameObject->getComponent<FakeComponent2>());
    ASSERT_TRUE(gameObject->getComponent<FakeComponent3>());
    ASSERT_TRUE(gameObject->getComponent<FakeComponent4>());
}
