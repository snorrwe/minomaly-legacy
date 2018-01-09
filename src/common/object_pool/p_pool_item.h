#pragma once
#include <iostream>

namespace Mino
{
	template <class T, size_t size> class IterablePool;

namespace Pool::Private
{

template <class T> class PoolItem : public T
{
public:
    size_t refIndex;
};

} // namespace Pool::Private

template <class T> class PoolItemRef
{
public:
    PoolItemRef() {}
    PoolItemRef(PoolItemRef const& r) = default;
    PoolItemRef(PoolItemRef&& r) = default;

    PoolItemRef& operator=(PoolItemRef const& r) = default;
    PoolItemRef& operator=(PoolItemRef&& r) = default;

	template <typename U, size_t size> friend class IterablePool;

    T& operator*() const { return *item; }
    T* operator->() const { return item; }

    operator T&() const { return *item; }
    operator T*() const { return item; }

private:
    void set(size_t ind, T* i)
    {
        item = i;
        poolIndex = ind;
    }

    T* item = nullptr;
    size_t poolIndex = 0;
};

} // namespace Mino
