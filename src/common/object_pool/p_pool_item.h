#pragma once
#include <cstddef>
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
    ManagedRef() {}
    ManagedRef(std::nullptr_t) {}
    ManagedRef(size_t ref, IterablePool<T>& pool) : refIndex(ref), pool(&pool) {}
    ManagedRef(ManagedRef const& mrf)
        : refIndex(mrf.refIndex), pool(mrf.pool), referenceCount(mrf.referenceCount)
    {
        ++*referenceCount;
    }
    ManagedRef(ManagedRef&& mrf)
        : refIndex(mrf.refIndex), pool(mrf.pool), referenceCount(mrf.referenceCount)
    {
        ++*referenceCount;
    }
    ~ManagedRef()
    {
        --*referenceCount;
        if (!*referenceCount && pool) pool->disable(refIndex);
    }

    ManagedRef& operator=(ManagedRef const&) = default;
    ManagedRef& operator=(ManagedRef&&) = default;

    void enable() const { pool->enable(refIndex); }
    void disable() const { pool->disable(refIndex); }

    operator size_t() const { return refIndex; }
    operator bool() const { return pool != nullptr; }

    T& operator*() const { return pool->get(refIndex); }
    T* operator->() const { return &(pool->get(refIndex)); }

    bool operator==(ManagedRef const& r) const { return r.refIndex == refIndex && r.pool == pool; }

private:
    size_t refIndex = 0;
    IterablePool<T>* pool = nullptr;
    std::shared_ptr<size_t> referenceCount = std::make_shared<size_t>(1);
};

} // namespace Mino
