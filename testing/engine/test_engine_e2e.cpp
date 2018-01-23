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

class FakeProgram : public Scene
{
public:
    FakeProgram(std::shared_ptr<IEngineCore> engine) : Scene(engine) {}

    virtual void start() {}

    void createHierarchy()
    {
        auto rootGo = createGameObject<FakeE2EComponent>();
        createHierarchy(rootGo);
    }

    void createHierarchy(GameObject* parent, size_t depth = 0)
    {
        if (childHierarchyLimit < ++depth) return;
        for (int i = 0; i < 50; ++i)
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
            engine->stop();
            return;
        }
    }

    void reset(size_t quitAfter = 1)
    {
        size_t updates = 0;
        this->quitAfter = quitAfter;
    }

    size_t updates = 0;
    size_t quitAfter = 1;
    size_t childHierarchyLimit = 10;
};

TEST_F(TestEngineE2E, CanCreateActual)
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

TEST_F(TestEngineE2E, CanCreateAndCleanUpGameObjects)
{
    auto engine = EngineCore::create<FakeProgram>("...", 0, 0);
    std::static_pointer_cast<FakeProgram>(engine->getScene())->createHierarchy();
    engine->run();
}
