#include "iterable_pool.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>

using namespace Mino;

class FakeType
{
public:
    FakeType() : thisCalls(0) {}
    FakeType(FakeType const& f) = default;
    FakeType(FakeType&& f) = default;

    FakeType& operator=(FakeType const&) = default;
    FakeType& operator=(FakeType&&) = default;

    void update()
    {
        thisCalls++;
        calls++;
    }

    size_t thisCalls = 0;
    static size_t calls;
};

size_t FakeType::calls = 0;

class ObjectPoolTests : public ::testing::Test
{
public:
    using Pool = IterablePool<FakeType, 50>;

    void SetUp() { FakeType::calls = 0; }

    Pool pool = Pool{};
};

TEST_F(ObjectPoolTests, CanCreate) {}

TEST_F(ObjectPoolTests, CanCreateItems)
{
    for (int i = 0; i < 5; ++i)
    {
        pool.create();
    }
}

TEST_F(ObjectPoolTests, CanIterate)
{
    for (int i = 0; i < 5; ++i)
    {
        pool.create();
    }
    FakeType::calls = 0;
    pool.iterateActive([](auto& i) { i.update(); });

    ASSERT_EQ(FakeType::calls, 5);
}

TEST_F(ObjectPoolTests, CanAccessReferencedItems)
{

    std::vector<Pool::Reference*> myItems{
        pool.create(), pool.create(), pool.create(), pool.create(), pool.create(),
    };

    // pool.remove(*myItems[2]);
    pool.iterateActive([](auto& i) { i.update(); });
    ASSERT_EQ(FakeType::calls, 5);
    for (auto i = myItems.begin(); i != myItems.end(); ++i)
    {
        ASSERT_EQ((**i)->thisCalls, 1);
    }
}

TEST_F(ObjectPoolTests, Method_iterateActive_DoesntIterateOnInavtive)
{

    std::vector<Pool::Reference*> myItems{
        pool.create(), pool.create(), pool.create(), pool.create(), pool.create(),
    };

    pool.remove(*myItems[2]);
    pool.iterateActive([](auto& i) { i.update(); });

    ASSERT_EQ(FakeType::calls, 4);
    ASSERT_EQ((*myItems[2])->thisCalls, 0);
}