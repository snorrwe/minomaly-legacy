#pragma once
#include "_observable.h"
#include "_subscription.h"

namespace Mino
{

template <typename T> class Subject : public Observable<T>
{
public:
	using Observable<T>::subscriptions;

    Subject() = default;
    Subject(Subject<T> const&) = delete;
    Subject(Subject<T>&&) = delete;
    virtual ~Subject() {}

    Subject<T>& operator=(Subject<T> const&) = delete;
    Subject<T>& operator=(Subject<T>&&) = delete;

    void next(T const& value)
    {
        for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
        {
            i->second(value);
        }
    }
};

} // namespace Mino
