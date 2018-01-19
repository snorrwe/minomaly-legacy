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
    size_t poolId;
};

template <class T> class WeakRef
{
public:
    WeakRef() {}
    WeakRef(std::nullptr_t) {}
    WeakRef(size_t ref, IterablePool<T>* pool) : refIndex(ref), pool(pool) {}
    WeakRef(WeakRef const&) = default;
    WeakRef(WeakRef&&) = default;
    virtual ~WeakRef() {}

    WeakRef& operator=(WeakRef const&) = default;
    WeakRef& operator=(WeakRef&&) = default;

    virtual void enable() const { pool->enable(refIndex); }
    virtual void disable() const { pool->disable(refIndex); }

    operator size_t() const { return refIndex; }
    operator bool() const { return pool; }

    T& operator*() const { return pool->get(refIndex); }
    T* operator->() const { return &(pool->get(refIndex)); }

    bool operator==(WeakRef const& r) const { return r.refIndex == refIndex && r.pool == pool; }

protected:
    size_t refIndex = 0;
    IterablePool<T>* pool = nullptr;
};

} // namespace Pool::Private

template <class T> class ManagedRef : public Pool::Private::WeakRef<T>
{
public:
    using WeakRef = Pool::Private::WeakRef<T>;

    ManagedRef() {}
    ManagedRef(std::nullptr_t) : WeakRef(nullptr) {}
    ManagedRef(size_t ref, IterablePool<T>* pool) : WeakRef(ref, pool) {}
    ManagedRef(ManagedRef const& mrf) : WeakRef(mrf), refs(mrf.refs)
    {
        if (refs) ++*refs;
    }
    ManagedRef(ManagedRef&& mrf) : WeakRef(mrf), refs(std::move(mrf.refs))
    {
        if (refs) ++*refs;
    }
    virtual ~ManagedRef()
    {
        if (refs && --*refs == 0 && pool)
        {
            pool->disable(refIndex);
        }
    }

    ManagedRef& operator=(ManagedRef const& mrf)
    {
        refIndex = mrf.refIndex;
        pool = mrf.pool;
        refs = mrf.refs;
        if (refs) ++*refs;
        return *this;
    }

    ManagedRef& operator=(ManagedRef&& mrf)
    {
        refIndex = std::move(mrf.refIndex);
        pool = std::move(mrf.pool);
        refs = std::move(mrf.refs);
        if (refs) ++*refs;
        return *this;
    }

    void enable() const { pool->enable(refIndex); }
    void disable() const { pool->disable(refIndex); }

    operator size_t() const { return refIndex; }
    operator bool() const { return *refs > 0 && pool; }
    operator WeakRef() const { return WeakRef(*this); }

    T& operator*() const { return pool->get(refIndex); }
    T* operator->() const { return &(pool->get(refIndex)); }

    bool operator==(ManagedRef const& r) const { return r.refIndex == refIndex && r.pool == pool; }

private:
    std::shared_ptr<size_t> refs = std::make_shared<size_t>(1);
};

} // namespace Mino
