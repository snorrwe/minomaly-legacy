#pragma once
#include <iostream>

namespace Mino
{

template <typename TValue = double> class Vector2
{
public:
    Vector2() = default;
    Vector2(TValue _x, TValue _y) : _x(_x), _y(_y) {}
    Vector2(Vector2 const&) = default;
    Vector2(Vector2&&) = default;
    ~Vector2() = default;

    Vector2<TValue>& operator=(Vector2<TValue> const&) = default;
    Vector2<TValue>& operator=(Vector2<TValue>&&) = default;

    Vector2<TValue> operator+(Vector2<TValue> const& other);
    Vector2<TValue> operator-(Vector2<TValue> const& other);

    TValue x() { return _x; }
    TValue y() { return _y; }

    template <typename T> friend std::ostream& operator<<(std::ostream& os, Vector2<T> const& v);

private:
    TValue _x = 0;
    TValue _y = 0;
};

template <typename TValue> Vector2<TValue> Vector2<TValue>::operator+(Vector2<TValue> const& other)
{
    return Vector2<TValue>(_x + other._x, _y + other._y);
}

template <typename TValue> Vector2<TValue> Vector2<TValue>::operator-(Vector2<TValue> const& other)
{
    return Vector2<TValue>(_x - other._x, _y - other._y);
}

template <typename T> std::ostream& operator<<(std::ostream& os, Vector2<T> const& v)
{
    os << v._x << " " << v._y;
    return os;
}

} // namespace Mino
