#include "render_system.h"
#include "gtest/gtest.h"
#include <memory>

using namespace Mino;

class RenderSystemTests : public ::testing::Test
{
public:
    void SetUp() { renderer = std::make_shared<RenderSystem>(nullptr); }

protected:
    std::shared_ptr<RenderSystem> renderer;
};

TEST_F(RenderSystemTests, CanCreateFromCtor) {}
