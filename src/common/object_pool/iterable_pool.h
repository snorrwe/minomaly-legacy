#pragma once
#include "p_pool_item.h"
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

namespace Mino
{

template <class T> class IterablePool
{
public:
    using PoolItem = Pool::Private::PoolItem<T>;
    using InternalRef = Pool::Private::PoolItemRef<T>;
    using Reference = ManagedRef<T>;

    IterablePool();
    virtual ~IterablePool();

    Reference enable();
    Reference enable(size_t id);
    void disable(size_t id);
    void disable(InternalRef& item);

    T& get(size_t id) { return pool[refs[id].poolIndex]; }

    void iterateActive(std::function<void(T&)> callback);
    void iterateAll(std::function<void(T&)> callback);

protected:
    size_t next = 0;
    std::vector<PoolItem> pool;
    std::vector<InternalRef> refs;

private:
    void swapItems(size_t index1, size_t index2);
};

template <class T> IterablePool<T>::IterablePool() {}

template <class T> IterablePool<T>::~IterablePool() {}

template <class T> typename IterablePool<T>::Reference IterablePool<T>::enable()
{
    if (++next < pool.size()) return IterablePool<T>::Reference(pool[next].refIndex, *this);
    pool.emplace_back();
    refs.emplace_back();
    auto index = pool.size() - 1;
    refs.back().set(index, &pool[index]);
    pool.back().refIndex = index;
    return IterablePool<T>::Reference(index, *this);
}

template <class T> typename IterablePool<T>::Reference IterablePool<T>::enable(size_t id)
{
    assert(id < refs.size());
    auto& ref = refs[id];
    if (ref.poolIndex > next)
    {
        swapItems(next, ref.poolIndex);
    }
    ++next;
    return IterablePool<T>::Reference(id, *this);
}

template <class T> void IterablePool<T>::disable(size_t index)
{
    auto poolIndex = refs[index].poolIndex;
    if (poolIndex < --next)
    {
        swapItems(poolIndex, next);
    }
}

template <class T> void IterablePool<T>::disable(typename IterablePool<T>::InternalRef& item)
{
    if (item.poolIndex < --next)
    {
        swapItems(item.poolIndex, next);
    }
}

template <class T> void IterablePool<T>::swapItems(size_t index1, size_t index2)
{
    assert(index1 < pool.size());
    assert(index2 < pool.size());
    using std::swap;

    swap(pool[index1], pool[index2]);
    refs[index2].set(index1, &pool[index1]);
    refs[index1].set(index2, &pool[index2]);
}

template <class T> void IterablePool<T>::iterateActive(std::function<void(T&)> callback)
{
    auto end = pool.begin() + next;
    for (auto i = pool.begin(); i != end; ++i)
    {
        callback(*i);
    }
}

template <class T> void IterablePool<T>::iterateAll(std::function<void(T&)> callback)
{
    for (auto i = pool.begin(); i != pool.end(); ++i)
    {
        callback(*i);
    }
}

} // namespace Mino
