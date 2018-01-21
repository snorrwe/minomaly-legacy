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
    WeakRef(size_t ref, IterablePool<T>* pool) : itemId(ref), pool(pool) {}
    WeakRef(WeakRef const&) = default;
    WeakRef(WeakRef&&) = default;
    virtual ~WeakRef() {}

    WeakRef& operator=(WeakRef const&) = default;
    WeakRef& operator=(WeakRef&&) = default;

    virtual void enable() const { pool->enable(itemId); }
    virtual void disable() const { pool->disable(itemId); }

    operator size_t() const { return itemId; }
    operator bool() const { return pool; }

    T& operator*() const { return pool->get(itemId); }
    T* operator->() const { return &(pool->get(itemId)); }

    bool operator==(WeakRef const& r) const { return r.itemId == itemId && r.pool == pool; }

protected:
    size_t itemId = 0;
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
        if (refs && --*refs == 0 && WeakRef::pool)
        {
            WeakRef::disable();
        }
    }

    ManagedRef& operator=(ManagedRef const& mrf)
    {
        WeakRef::operator=(mrf);
        refs = mrf.refs;
        if (refs) ++*refs;
        return *this;
    }

    ManagedRef& operator=(ManagedRef&& mrf)
    {
        refs = std::move(mrf.refs);
        if (refs) ++*refs;
        WeakRef::operator=(std::move(mrf));
        return *this;
    }

    WeakRef& release()
    { /*
         Releases control of the managed object.
         Note: if there are copies of the Reference they will still own the item
     */
        refs = nullptr;
        return *this;
    }

private:
    std::shared_ptr<size_t> refs = std::make_shared<size_t>(1);
};

} // namespace Mino
