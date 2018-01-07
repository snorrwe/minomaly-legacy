#pragma once
#include "p_pool_item.h"
#include <array>
#include <cassert>
#include <functional>
#include <memory>

namespace Mino
{

template <class T, size_t size = 50> class IterablePool
{
public:
    using PoolItem = Pool::Private::PoolItem<T>;
    using Reference = PoolItemRef<T>;

    IterablePool();
    virtual ~IterablePool();

    Reference* enable();
    Reference* enable(Reference& item);
    void disable(Reference& item);
    bool canCreate();

    void iterateActive(std::function<void(T&)> callback);
    void iterateAll(std::function<void(T&)> callback);

protected:
    size_t next = 0;
    std::array<PoolItem, size> pool;
    std::array<Reference, size> refs;

private:
    void swapItems(size_t index1, size_t index2);
};

template <class T, size_t size> IterablePool<T, size>::IterablePool()
{
    for (int i = 0; i < size; ++i)
    {
        pool[i].refIndex = i;
        refs[i].set(i, &pool[i]);
        assert(refs[i].item == &pool[i]);
    }
}

template <class T, size_t size> IterablePool<T, size>::~IterablePool() {}

template <class T, size_t size>
typename IterablePool<T, size>::Reference* IterablePool<T, size>::enable()
{
    assert(next < size);
    auto& result = pool[next];
    ++next;
    return &refs[result.refIndex];
}

template <class T, size_t size>
typename IterablePool<T, size>::Reference* IterablePool<T, size>::enable(Reference& ref)
{
    auto result = &refs[pool[ref.poolIndex].refIndex];
    if (ref.poolIndex < size) return result;
    swapItems(++next, result->poolIndex);
    return result;
}

template <class T, size_t size>
void IterablePool<T, size>::disable(typename IterablePool<T, size>::Reference& item)
{
    if (item.poolIndex < --next)
    {
        swapItems(item.poolIndex, next);
    }
}

template <class T, size_t size> bool IterablePool<T, size>::canCreate() { return next < size; }

template <class T, size_t size> void IterablePool<T, size>::swapItems(size_t index1, size_t index2)
{
    assert(index1 < size);
    assert(index2 < size);
    using std::swap;

    swap(pool[index1], pool[index2]);
    refs[index2].set(index1, &pool[index1]);
    refs[index1].set(index2, &pool[index2]);
}

template <class T, size_t size>
void IterablePool<T, size>::iterateActive(std::function<void(T&)> callback)
{
    auto end = pool.begin() + next;
    for (auto i = pool.begin(); i != end; ++i)
    {
        callback(*i);
    }
}

template <class T, size_t size>
void IterablePool<T, size>::iterateAll(std::function<void(T&)> callback)
{
    for (auto& i = pool.begin(); i != pool.end(); ++i)
    {
        callback(*i);
    }
}

} // namespace Mino
