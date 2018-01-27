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
    using Reference = ManagedRef<T>;

    IterablePool(size_t count = 0);
    IterablePool(IterablePool const&) = delete; // TODO
    IterablePool(IterablePool&&) = delete;
    virtual ~IterablePool();

    IterablePool& operator=(IterablePool const&) = delete; // TODO
    IterablePool& operator=(IterablePool&&) = delete;

    Reference enable();
    Reference enable(size_t id);
    void disable(size_t id);

    size_t size() { return pool.size(); }
    size_t enabled() { return next; }

    bool isEnabled(size_t id);

    T& get(size_t id) { return pool[refs[id]]; }

    void iterateActive(std::function<void(T&)> callback);
    void iterateAll(std::function<void(T&)> callback);

protected:
    void add();

    size_t next = 0;
    std::vector<PoolItem> pool = {};
    std::vector<size_t> refs = {};

private:
    void swapItems(size_t index1, size_t index2);
};

template <class T> IterablePool<T>::IterablePool(size_t count)
{
    for (int i = 0; i < count; ++i)
    {
        add();
    }
}

template <class T> IterablePool<T>::~IterablePool()
{
    next = 0; // This fixes a lifetime management issue in Release configuration
}

template <class T> void IterablePool<T>::add()
{
    pool.emplace_back();
    refs.emplace_back();
    auto index = pool.size() - 1;
    refs.back() = index;
    pool.back().poolId = index;
}

template <class T> typename IterablePool<T>::Reference IterablePool<T>::enable()
{
    if (next >= pool.size()) add();

    return IterablePool<T>::Reference(next++, this);
}

template <class T> typename IterablePool<T>::Reference IterablePool<T>::enable(size_t id)
{
    assert(id < refs.size());
    swapItems(next++, refs[id]);
    return IterablePool<T>::Reference(id, this);
}

template <class T> void IterablePool<T>::disable(size_t id)
{
    assert(id < refs.size());
    if (next > 0) swapItems(refs[id], --next);
}

template <class T> void IterablePool<T>::swapItems(size_t index1, size_t index2)
{
    assert(index1 < pool.size());
    assert(index2 < pool.size());

    auto tmp = std::move(pool[index1]);
    pool[index1] = std::move(pool[index2]);
    pool[index2] = std::move(tmp);
    refs[index2] = index1;
    refs[index1] = index2;
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
    for (auto& i : pool)
    {
        callback(i);
    }
}

template <class T> bool IterablePool<T>::isEnabled(size_t id)
{
    return id < refs.size() && refs[id] < next;
}

} // namespace Mino
