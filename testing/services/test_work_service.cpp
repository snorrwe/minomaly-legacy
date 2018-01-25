#include "work_service.h"
#include "gtest/gtest.h"

using namespace Mino;

class WorkServiceTests : public ::testing::Test
{
public:
    void SetUp() { workService = WorkService::create(); }
    void TearDown() { workService = nullptr; }

    std::shared_ptr<WorkService> workService = nullptr;
};

TEST_F(WorkServiceTests, CanCreate) { ASSERT_TRUE(workService); }

TEST_F(WorkServiceTests, CanRequestArbitraryWork)
{
    auto future = workService->requestWork<int>([]() { return 5; });

    future.wait();

    ASSERT_EQ(future.get(), 5);
}

TEST_F(WorkServiceTests, CanRequestArbitraryJobs)
{
    auto future1 = workService->requestWork<int>([]() { return 5; });
    auto future2 = workService->requestWork<std::string>([]() { return "Winnie"; });
    auto future3 = workService->requestWork<float>([]() { return 5.6f; });

    future1.wait();
    future2.wait();
    future3.wait();

    ASSERT_EQ(future1.get(), 5);
    ASSERT_EQ(future2.get(), "Winnie");
    ASSERT_FLOAT_EQ(future3.get(), 5.6f);
}

TEST_F(WorkServiceTests, CanRequestBunchOfWork)
{
    auto futures = std::vector<std::future<int>>{};
    for (auto i = 0; i < 500; ++i)
    {
        futures.push_back(workService->requestWork<int>([]() { return 5; }));
    }

    for (auto& future : futures)
    {
        future.wait();
        ASSERT_EQ(future.get(), 5);
    }
}
