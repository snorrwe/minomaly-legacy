#include "mino.h"
#include "gtest/gtest.h"
#include <memory>

using namespace Mino;

class TestEngineE2E : public ::testing::Test
{
public:
};

class FakeE2EComponent : public Component
{
public:
};

class FakeProgram : public Application
{
public:
    virtual void start() {}

    void createHierarchy()
    {
        auto rootGo = createGameObject<FakeE2EComponent>();
        createHierarchy(rootGo);
    }

    void createHierarchy(GameObject* parent, size_t depth = 0)
    {
        if (childHierarchyLimit < ++depth) return;
        for (int i = 0; i < 13; ++i)
        {
            auto go = createGameObject<FakeE2EComponent>();
            parent->addChild(*go);
            createHierarchy(go, depth);
        }
    }

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
    size_t childHierarchyLimit = 5;
};

TEST_F(TestEngineE2E, CanCreateActual)
{
    /*
        Does not test Mixer status, as AppVoyer does not support audio endpoints
        TODO: find a way to test the audio system on the CI
    */
    auto engine = EngineCore::create<FakeProgram>("...", 0, 0);
    EXPECT_EQ(engine->subsystemStatus(SdlSubSystemType::SDL), SdlStatus::Initialized);
    EXPECT_EQ(engine->subsystemStatus(SdlSubSystemType::SDL_image), SdlStatus::Initialized);
    EXPECT_EQ(engine->subsystemStatus(SdlSubSystemType::SDL_ttf), SdlStatus::Initialized);
}

TEST_F(TestEngineE2E, CanCreateAndCleanUpGameObjects)
{
    auto engine = EngineCore::create<FakeProgram>("...", 0, 0);
    static_cast<FakeProgram*>(engine->getApplication())->createHierarchy();
    engine->run();
}
