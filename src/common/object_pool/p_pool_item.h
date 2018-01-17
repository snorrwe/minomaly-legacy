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

} // namespace Pool::Private

template <class T> class ManagedRef
{
public:
    ManagedRef() {}
    ManagedRef(std::nullptr_t) {}
    ManagedRef(size_t ref, IterablePool<T>* pool) : refIndex(ref), pool(pool) {}
    ManagedRef(ManagedRef const& mrf) : refIndex(mrf.refIndex), pool(mrf.pool), refs(mrf.refs)
    {
        if (refs) ++*refs;
    }
    ManagedRef(ManagedRef&& mrf)
        : refIndex(std::move(mrf.refIndex)), pool(std::move(mrf.pool)), refs(std::move(mrf.refs))
    {
        if (refs) ++*refs;
    }
    ~ManagedRef()
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

    T& operator*() const { return pool->get(refIndex); }
    T* operator->() const { return &(pool->get(refIndex)); }

    bool operator==(ManagedRef const& r) const { return r.refIndex == refIndex && r.pool == pool; }

private:
    size_t refIndex = 0;
    IterablePool<T>* pool = nullptr;
    std::shared_ptr<size_t> refs = std::make_shared<size_t>(1);
};

} // namespace Mino
