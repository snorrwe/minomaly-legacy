#include "core.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

using ::testing::AtLeast;
using ::testing::NiceMock;
using namespace Mino;

class MockSubsystems : public SdlSubsystems
{
public:
    MockSubsystems(std::shared_ptr<ILogService> l) : SdlSubsystems(l) {}
};

class MockWindow : public IWindowSystem
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

class MockLogService : public ILogService
{
public:
    MOCK_METHOD1(error, void(std::string const& message));
    MOCK_METHOD1(warning, void(std::string const& message));
    MOCK_METHOD1(debug, void(std::string const& message));
    MOCK_METHOD1(info, void(std::string const& message));
    MOCK_METHOD1(setFname, void(std::string const& fname));
    MOCK_METHOD0(getFname, std::string());
};

class MockTimeSystem : public ITimeService
{
public:
    MOCK_CONST_METHOD0(now, TimePoint());
    MOCK_CONST_METHOD0(deltaTime, double());

protected:
    MOCK_METHOD1(update, void(TimePoint const&));
};

class MockPhysics : public IPhysicsSystem
{
public:
    MOCK_METHOD0(update, void());
    MOCK_METHOD0(getWorld, std::shared_ptr<Quadtree<Collider>>());
    MOCK_METHOD1(add, void(std::weak_ptr<Collider> const&));
    MOCK_METHOD1(remove, void(std::weak_ptr<Collider> const&));
};

class FakeProgram : public Scene
{
public:
    FakeProgram(std::shared_ptr<EngineCore> engine) : Scene(engine) {}
    virtual ~FakeProgram() {}

    virtual void start() { starts++; }
    virtual void update()
    {
        updates++;
        if (updates >= quitAfter)
        {
            engine->stop();
            return;
        }
    }

    void reset(size_t quitAfter = 1)
    {
        size_t updates = 0;
        size_t starts = 0;
        this->quitAfter = quitAfter;
    }

    size_t updates = 0;
    size_t starts = 0;
    size_t quitAfter = 1;
};

class CoreTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        auto mockLogService = std::make_shared<NiceMock<MockLogService>>();
        mockSubsystems = std::make_shared<NiceMock<MockSubsystems>>(mockLogService);
        mockInput = std::make_shared<NiceMock<MockInput>>();
        mockRenderer = std::make_shared<NiceMock<MockRenderer>>();
        mockWindow = std::make_shared<NiceMock<MockWindow>>();
        mockPhysics = std::make_shared<NiceMock<MockPhysics>>();
        mockAudioSystem = std::make_shared<NiceMock<MockAudioSystem>>();
        mockTimeSystem = std::make_shared<NiceMock<MockTimeSystem>>();
        engine = std::make_shared<EngineCore>(mockSubsystems, mockInput, mockWindow, mockRenderer,
                                              mockAudioSystem, mockPhysics, mockLogService,
                                              mockTimeSystem);
        fakeProgram = std::make_shared<FakeProgram>(engine);
        engine->setScene(fakeProgram);
    }

    /*Acutal*/
    std::shared_ptr<EngineCore> engine;

    /*Mocks*/
    std::shared_ptr<MockSubsystems> mockSubsystems;
    std::shared_ptr<MockInput> mockInput;
    std::shared_ptr<MockRenderer> mockRenderer;
    std::shared_ptr<FakeProgram> fakeProgram;
    std::shared_ptr<MockWindow> mockWindow;
    std::shared_ptr<MockPhysics> mockPhysics;
    std::shared_ptr<MockAudioSystem> mockAudioSystem;
    std::shared_ptr<MockTimeSystem> mockTimeSystem;
};

TEST(TestCreate, CanCreateActual)
{
    /*
        Does not test Mixer status, as AppVoyer does not support audio endpoints
        TODO: find a way to test the audio system on the CI
    */
    auto engine = EngineCore::create<FakeProgram>("...", 0, 0);
    auto status = engine->subsystemStatus({
        SdlSubSystemType::SDL,
        SdlSubSystemType::SDL_image,
        SdlSubSystemType::SDL_ttf,
    });
    for (auto i = status.begin(); i != status.end(); ++i)
    {
        ASSERT_EQ(*i, SdlStatus::Initialized);
    }
}

TEST_F(CoreTest, CanCreate) { ASSERT_TRUE(engine); }

TEST_F(CoreTest, CanStopEngineFromLogic)
{
    fakeProgram->quitAfter = 1;
    EXPECT_CALL(*mockInput, update()).Times(AtLeast(1));
    EXPECT_CALL(*mockRenderer, update()).Times(AtLeast(1));

    engine->run();

    ASSERT_EQ(fakeProgram->starts, 1);
    ASSERT_GE(fakeProgram->updates, 1);
}

TEST_F(CoreTest, CallsLogicStartOnce)
{
    fakeProgram->quitAfter = 10;
    engine->run();
    ASSERT_EQ(fakeProgram->starts, 1);
    ASSERT_GE(fakeProgram->updates, 10);
}
