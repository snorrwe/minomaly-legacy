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
    static constexpr size_t poolSize = 50;
    using Pool = IterablePool<FakeType, poolSize>;

    void SetUp() { FakeType::calls = 0; }

    void someCallbackExpectingFake(FakeType const& f) {}
    void someCallbackExpectingFakePtr(FakeType const* f) {}

    Pool pool = Pool{};
};

TEST_F(ObjectPoolTests, CanCreate) {}

TEST_F(ObjectPoolTests, CanCreateItems)
{
    for (int i = 0; i < 5; ++i)
    {
        pool.enable();
    }
}

TEST_F(ObjectPoolTests, CanIterate)
{
    for (int i = 0; i < 5; ++i)
    {
        pool.enable();
    }
    FakeType::calls = 0;
    pool.iterateActive([](auto& i) { i.update(); });

    ASSERT_EQ(FakeType::calls, 5);
}

TEST_F(ObjectPoolTests, CanAccessReferencedItems)
{
    std::vector<Pool::Reference*> myItems{
        pool.enable(), pool.enable(), pool.enable(), pool.enable(), pool.enable(),
    };

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
        pool.enable(), pool.enable(), pool.enable(), pool.enable(), pool.enable(),
    };

    pool.disable(*myItems[2]);
    pool.iterateActive([](auto& i) { i.update(); });

    ASSERT_EQ(FakeType::calls, 4);
    ASSERT_EQ((*myItems[2])->thisCalls, 0);
}

TEST_F(ObjectPoolTests, Method_iterate_iteratesOnInactiveToo)
{
    std::vector<Pool::Reference*> myItems{
        pool.enable(), pool.enable(), pool.enable(), pool.enable(), pool.enable(),
    };

    for (auto i = myItems.begin(); i != myItems.end(); ++i)
    {
        pool.disable(**i);
    }

    pool.iterateAll([](auto& i) { i.update(); });

    ASSERT_EQ(FakeType::calls, poolSize);
    for (auto i = myItems.begin(); i != myItems.end(); ++i)
    {
        ASSERT_EQ((**i)->thisCalls, 1);
    }
}

TEST_F(ObjectPoolTests, ReferencesShouldConvertToUnderlyingTypeImplicitly)
{

    auto ref = pool.enable();
    someCallbackExpectingFake(*ref);
    someCallbackExpectingFakePtr(*ref);
}

TEST_F(ObjectPoolTests, CanReenableAlreadyUsedItem)
{

    auto ref1 = pool.enable();
    auto ref2 = ref1;

    pool.disable(*ref1);
    ref1 = pool.enable(*ref1);

    ASSERT_EQ(ref1, ref2);
}
