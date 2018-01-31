#pragma once
#include "_subscription.h"
#include <functional>
#include <map>

namespace Mino
{

template <typename T> class Observable
{
public:
    typedef Subscription<T> Subscription;

    Observable() : subscriptions() {}
    Observable(Observable<T>&&) = delete;
    Observable(Observable<T> const&) = delete;
    virtual ~Observable() {}

    Observable<T>& operator=(Observable<T> const&) = delete;
    Observable<T>& operator=(Observable<T>&&) = delete;

    ISubscription subscribe(std::function<void(T const&)> cb)
    {
        subscriptions[nextSub] = cb;
        return Subscription(*this, nextSub++);
    }

    void unsubscribe(size_t key) { subscriptions.erase(key); }

protected:
    std::map<size_t, std::function<void(T const&)>> subscriptions;
    size_t nextSub = 0;
};

} // namespace Mino
