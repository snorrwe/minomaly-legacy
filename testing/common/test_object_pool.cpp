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
    using Pool = IterablePool<FakeType>;

    void SetUp() { FakeType::calls = 0; }

    void someCallbackExpectingFake(FakeType const& f) {}

    std::unique_ptr<Pool> pool = std::make_unique<Pool>();
};

TEST_F(ObjectPoolTests, CanCreate) {}

TEST_F(ObjectPoolTests, CanCreateItems)
{
    for (int i = 0; i < 5; ++i)
    {
        pool->enable();
    }
}

TEST_F(ObjectPoolTests, CanCreateItem) { auto ref = pool->enable(); }

TEST_F(ObjectPoolTests, CanIterate)
{
    std::vector<Pool::Reference> references;
    for (int i = 0; i < 5; ++i)
    {
        references.push_back(pool->enable());
    }
    FakeType::calls = 0;
    pool->foreachActive([](auto& i) { i.update(); });

    ASSERT_EQ(FakeType::calls, 5);
}

TEST_F(ObjectPoolTests, CanAccessReferencedItems)
{
    std::vector<Pool::Reference> myItems{
        pool->enable(), pool->enable(), pool->enable(), pool->enable(), pool->enable(),
    };

    pool->foreachActive([](auto& i) { i.update(); });
    ASSERT_EQ(FakeType::calls, 5);
    for (auto i = myItems.begin(); i != myItems.end(); ++i)
    {
        ASSERT_EQ((*i)->thisCalls, 1);
    }
}

TEST_F(ObjectPoolTests, Method_iterateActive_DoesntIterateOnInactive)
{

    std::vector<Pool::Reference> myItems{
        pool->enable(), pool->enable(), pool->enable(), pool->enable(), pool->enable(),
    };
    myItems[2].disable();
    pool->foreachActive([](auto& i) { i.update(); });

    ASSERT_EQ(FakeType::calls, 4);
    ASSERT_EQ((*myItems[2]).thisCalls, 0);
}

TEST_F(ObjectPoolTests, Method_iterate_iteratesOnInactiveToo)
{
    size_t poolSize = 50;
    std::vector<Pool::Reference> myItems{};
    for (int i = 0; i < poolSize; ++i)
    {
        myItems.push_back(pool->enable());
    }

    for (auto i = myItems.begin(); i != myItems.end(); ++i)
    {
        i->disable();
    }

    pool->foreach([](auto& i) { i.update(); });

    ASSERT_EQ(FakeType::calls, poolSize);
    for (auto i = myItems.begin(); i != myItems.end(); ++i)
    {
        ASSERT_EQ((*i)->thisCalls, 1);
    }
}

TEST_F(ObjectPoolTests, ReferencesShouldConvertToUnderlyingTypeImplicitly)
{
    auto ref = pool->enable();
    someCallbackExpectingFake(*ref);
}

TEST_F(ObjectPoolTests, CanReenableAlreadyUsedItem)
{
    auto ref1 = pool->enable();

    ref1.disable();
    ref1.enable();
}
