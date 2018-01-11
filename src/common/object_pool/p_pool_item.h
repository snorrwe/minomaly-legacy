#pragma once
#include <iostream>
#include <memory>

namespace Mino
{
template <class T> class IterablePool;

namespace Pool::Private
{

template <class T> class PoolItem final : public T
{
public:
    template <typename U> friend class IterablePool;

private:
    size_t refIndex;
};

template <class T> class PoolItemRef
{
public:
    PoolItemRef() {}
    PoolItemRef(PoolItemRef const& r) = default;
    PoolItemRef(PoolItemRef&& r) = default;

    PoolItemRef& operator=(PoolItemRef const& r) = default;
    PoolItemRef& operator=(PoolItemRef&& r) = default;

    template <typename U> friend class IterablePool;

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

} // namespace Pool::Private

template <class T> class ManagedRef
{
public:
    static std::shared_ptr<ManagedRef<T>> create(size_t ref, IterablePool<T>& pool)
    {
        return std::make_shared<ManagedRef<T>>(ref, pool);
    }

    ManagedRef(size_t ref, IterablePool<T>& pool) : refIndex(ref), pool(pool) {}
    ~ManagedRef() { disable(); }

    void enable() { pool.enable(refIndex); }
    void disable() { pool.disable(refIndex); }

    operator size_t() { return refIndex; }

    T& operator*() const { return pool.get(refIndex); }
    T* operator->() const { return &(pool.get(refIndex)); }

private:
    size_t refIndex;
    IterablePool<T>& pool;
};

} // namespace Mino
