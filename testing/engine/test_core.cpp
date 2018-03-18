#include "core.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::_;
using namespace Mino;

class MockSubsystems : public SdlSubsystems
{
public:
    MockSubsystems(std::shared_ptr<ILogService> l)
        : SdlSubsystems(l)
    {
    }
};

class MockWindow : public IWindowSystem
{
public:
    MOCK_METHOD4(fillRect, void(const SDL_Rect*, uint8_t, uint8_t, uint8_t));
    MOCK_METHOD3(blitSurface, void(SDL_Surface*, const SDL_Rect*, SDL_Rect*));
    MOCK_METHOD3(blitScaled, void(SDL_Surface*, const SDL_Rect*, SDL_Rect*));
    MOCK_METHOD0(size, Vector2<int>());
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
    MOCK_METHOD0(addCamera, IRenderSystem::CameraReferences::Reference());
    MOCK_METHOD0(getMainCamera, IRenderSystem::CameraReference&());
    MOCK_CONST_METHOD0(getMainCamera, IRenderSystem::CameraReference const&());
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
    MOCK_CONST_METHOD0(deltaTime, float());

protected:
    MOCK_METHOD1(update, void(TimePoint const&));
};

class MockPhysics : public IPhysicsSystem
{
public:
    MOCK_METHOD0(update, void());
    MOCK_METHOD1(add, void(Collider*));
    MOCK_METHOD1(remove, void(Collider*));
    MOCK_METHOD1(setWorldBox, void(BoundingBox const&));
    MOCK_METHOD0(getWorld, Quadtree<Collider>*());
};

class MockAssets : public IAssetSystem
{
public:
    MOCK_METHOD1(loadTexture, std::shared_ptr<Texture>(std::string const& name));
    MOCK_METHOD3(loadText,
                 std::shared_ptr<Texture>(std::string const& text, Font const& font, Color const&));
    MOCK_METHOD2(loadSpriteSheet,
                 std::shared_ptr<TSpriteSheet>(std::string const& name,
                                               std::vector<SDL_Rect> const& rects));
    MOCK_METHOD2(loadFont, std::shared_ptr<Font>(std::string const& path, int pts));
    MOCK_METHOD0(collectGarbage, void());
    MOCK_METHOD0(clear, void());
    MOCK_METHOD1(loadWAV, std::shared_ptr<Audio>(std::string const&));
    MOCK_METHOD1(loadMusic, std::shared_ptr<Music>(std::string const&));
};

class FakeProgram : public Application
{
public:
    virtual void update()
    {
        updates++;
        if (updates >= quitAfter)
        {
            getEngineCore()->stop();
            return;
        }
    }

    void reset(size_t quitAfter = 1)
    {
        updates = 0;
        this->quitAfter = quitAfter;
    }

    size_t updates = 0;
    size_t quitAfter = 1;
};

class MockApplication : public Application
{
public:
    MOCK_METHOD0(update, void());
    MOCK_METHOD0(start, void());
    MOCK_METHOD1(setEngineCore, void(EngineCore*));

    void delegateToFake()
    {
        ON_CALL(*this, update()).WillByDefault(Invoke(&fake, &FakeProgram::update));
        ON_CALL(*this, setEngineCore(_)).WillByDefault(Invoke(&fake, &FakeProgram::setEngineCore));
    }

    FakeProgram& getFake() { return fake; }

private:
    FakeProgram fake;
};

class CoreTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        auto mockApp = std::make_unique<NiceMock<MockApplication>>();
        mockApplication = mockApp.get();
        mockApplication->delegateToFake();

        auto mockInp = std::make_unique<NiceMock<MockInput>>();
        mockInput = mockInp.get();

        mockLogService = std::make_shared<NiceMock<MockLogService>>();
        auto mockRender = std::make_unique<NiceMock<MockRenderer>>();
        mockRenderer = mockRender.get();
        auto mockWindow = std::make_unique<NiceMock<MockWindow>>();
        auto mockPhysics = std::make_unique<NiceMock<MockPhysics>>();
        auto mockAudioSystem = std::make_unique<NiceMock<MockAudioSystem>>();
        auto mockAssets = std::make_unique<MockAssets>();
        mockTimeSystem = std::make_shared<NiceMock<MockTimeSystem>>();
        engine = std::make_unique<EngineCore>(
            std::make_unique<NiceMock<MockSubsystems>>(mockLogService),
            std::move(mockInp),
            std::move(mockWindow),
            std::move(mockApp),
            std::move(mockRender),
            std::move(mockAudioSystem),
            std::move(mockPhysics),
            std::move(mockAssets),
            mockLogService,
            mockTimeSystem);
        mockApplication->setEngineCore(engine.get());
    }

    /*Acutal*/
    std::unique_ptr<EngineCore> engine;

    /*Mocks*/
    MockApplication* mockApplication;
    MockInput* mockInput;
    MockRenderer* mockRenderer;
    std::shared_ptr<MockTimeSystem> mockTimeSystem;
    std::shared_ptr<MockLogService> mockLogService;
};

TEST_F(CoreTest, CanCreate) { ASSERT_TRUE(engine); }

TEST_F(CoreTest, CanStopEngineFromLogic)
{
    EXPECT_CALL(*mockInput, update()).Times(AtLeast(1));
    EXPECT_CALL(*mockRenderer, update()).Times(AtLeast(1));
    EXPECT_CALL(*mockApplication, start()).Times(1);
    EXPECT_CALL(*mockApplication, update()).Times(1);

    mockApplication->getFake().quitAfter = 1;
    engine->run();
}

TEST_F(CoreTest, CallsLogicStartOnce)
{
    EXPECT_CALL(*mockApplication, start()).Times(1);
    EXPECT_CALL(*mockApplication, update()).Times(10);

    mockApplication->getFake().quitAfter = 10;
    engine->run();
}
