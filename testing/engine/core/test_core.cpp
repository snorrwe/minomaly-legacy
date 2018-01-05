#include "core.h"
#include "gtest/gtest.h"
#include <memory>

class CoreTest : public ::testing::Test
{
public:
    class TestProgram : public ILogic
    {
    public:
        TestProgram(std::shared_ptr<Core> core) : ILogic(core) {}
        virtual ~TestProgram() {}

        virtual void start() {}
        virtual void update() {}
    };
};

TEST_F(CoreTest, CanCreateActual) { Core::create<CoreTest::TestProgram>("...", 0, 0); }
