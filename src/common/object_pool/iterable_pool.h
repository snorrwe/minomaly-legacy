#pragma once
#include "p_pool_item.h"
#include <cassert>
#include <functional>
#include <memory>
#include <vector>

namespace Mino
{

template <class T> class IterablePool
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
    std::vector<PoolItem> pool;
    std::vector<Reference> refs;

private:
    void swapItems(size_t index1, size_t index2);
};

template <class T> IterablePool<T>::IterablePool() {}

template <class T> IterablePool<T>::~IterablePool() {}

template <class T> typename IterablePool<T>::Reference* IterablePool<T>::enable()
{
    if (++next < pool.size()) return &refs[pool[next].refIndex];
    pool.emplace_back();
    refs.emplace_back();
    auto index = pool.size() - 1;
    refs.back().set(index, &pool[index]);
    pool.back().refIndex = index;
    return &refs.back();
}

template <class T> typename IterablePool<T>::Reference* IterablePool<T>::enable(Reference& ref)
{
    auto result = &refs[pool[ref.poolIndex].refIndex];
    if (ref.poolIndex < next) return result;
    swapItems(++next, result->poolIndex);
    return result;
}

template <class T> void IterablePool<T>::disable(typename IterablePool<T>::Reference& item)
{
    if (item.poolIndex < --next)
    {
        swapItems(item.poolIndex, next);
    }
}

template <class T> bool IterablePool<T>::canCreate() { return next < size; }

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
