#include "application.h"
#include "component.h"
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
    MOCK_CONST_METHOD0(getApplication, std::shared_ptr<Application>());
    MOCK_CONST_METHOD0(getPhysicsSystem, std::shared_ptr<IPhysicsSystem>());
    MOCK_CONST_METHOD0(getTime, std::shared_ptr<ITimeService>());
    MOCK_CONST_METHOD1(subsystemStatus, SdlStatus(SdlSubSystemType));
    MOCK_CONST_METHOD1(subsystemStatus,
                       std::vector<SdlStatus>(std::vector<SdlSubSystemType> const&));
    MOCK_METHOD1(setTargetFps, void(float f));
};

class FakeScene : public Application
{
public:
    ~FakeScene() {}
    void update() {}
};

class SceneTests : public ::testing::Test
{
public:
    void SetUp()
    {
        mockEngineCore = std::make_shared<MockEngineCore>();
        application.setEngineCore(mockEngineCore.get());
    }

    FakeScene application = {};
    std::shared_ptr<MockEngineCore> mockEngineCore = nullptr;
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
    auto go = application.createGameObject<>();
    ASSERT_EQ(go->getApplication(), &application);
}

TEST_F(SceneTests, CanCreateGameObjectWithComponents)
{
    auto gameObject =
        application
            .createGameObject<FakeComponent1, FakeComponent2, FakeComponent3, FakeComponent4>();

    ASSERT_TRUE(gameObject->getComponent<FakeComponent1>());
    ASSERT_TRUE(gameObject->getComponent<FakeComponent2>());
    ASSERT_TRUE(gameObject->getComponent<FakeComponent3>());
    ASSERT_TRUE(gameObject->getComponent<FakeComponent4>());
}
