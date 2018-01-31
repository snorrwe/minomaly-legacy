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
    virtual bool enabled() const { return pool->isEnabled(itemId); }
    virtual T& get() const { return pool->get(itemId); }

    operator bool() const { return pool != nullptr; }

    T& operator*() const { return get(); }
    T* operator->() const { return &get(); }

    bool operator==(WeakRef const& r) const { return r.itemId == itemId && r.pool == pool; }

protected:
    size_t itemId = 0;
    IterablePool<T>* pool = nullptr;
};

template <class T> class ManagedRef : public WeakRef<T>
{
public:
    using WeakRef = WeakRef<T>;

    ManagedRef() {}
    ManagedRef(std::nullptr_t) : WeakRef(nullptr) {}
    ManagedRef(size_t ref, IterablePool<T>* pool) : WeakRef(ref, pool) {}
    ManagedRef(ManagedRef const& mrf) : WeakRef(mrf), refs(mrf.refs)
    {
        if (refs) ++(*refs);
    }
    ManagedRef(ManagedRef&& mrf) : WeakRef(mrf), refs(std::move(mrf.refs)) {}
    virtual ~ManagedRef()
    {
        if (!refs || !WeakRef::pool) return;
        --(*refs);
        if (!*refs)
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
        WeakRef::operator=(std::move(mrf));
        return *this;
    }

    WeakRef& release()
    { /*
         Releases control of the managed object.
         Note: if there are copies of the Reference they will still own the item
     */
        --*refs;
        refs = nullptr;
        return *this;
    }

private:
    std::shared_ptr<size_t> refs = std::make_shared<size_t>(1);
};

} // namespace Mino
