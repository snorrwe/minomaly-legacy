#pragma once
#include "_observable.h"

namespace Mino
{

template <typename T> class Observable;

class ISubscription
{
public:
    virtual ~ISubscription() {}

    virtual void unsubscribe() const {};
};

template <typename T> class Subscription : public ISubscription
{
public:
    Subscription(Observable<T>& o, size_t key) : observable(&o), key(key) {}
    Subscription(Subscription<T> const&) = default;
    Subscription(Subscription<T>&&) = default;
    virtual ~Subscription() {}

    Subscription<T>& operator=(Subscription const&) = default;
    Subscription<T>& operator=(Subscription&&) = default;

    virtual void unsubscribe() const { observable->unsubscribe(key); }

private:
    Subscription() = default;

    Observable<T>* observable;
    size_t key;
};

} // namespace Mino
