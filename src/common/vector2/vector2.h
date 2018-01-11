#pragma once
#include <cmath>
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

    Vector2<TValue> operator+(Vector2<TValue> const& other) const;
    Vector2<TValue> operator-(Vector2<TValue> const& other) const;
    Vector2<TValue> operator*(double);
    bool operator==(Vector2<TValue> const& other) const;
    bool operator!=(Vector2<TValue> const& other) const { return !(*this == other); }

    TValue x() const { return _x; }
    TValue y() const { return _y; }
    TValue length() const { return sqrt(_x * _x + _y * _y); }

    void rotate(const double rads);
    void rotateDeg(const double degrees);

    template <typename T> friend std::ostream& operator<<(std::ostream& os, Vector2<T> const& v);

private:
    TValue _x = 0;
    TValue _y = 0;
};

template <typename TValue>
Vector2<TValue> Vector2<TValue>::operator+(Vector2<TValue> const& other) const
{
    return Vector2<TValue>(_x + other._x, _y + other._y);
}

template <typename TValue>
Vector2<TValue> Vector2<TValue>::operator-(Vector2<TValue> const& other) const
{
    return Vector2<TValue>(_x - other._x, _y - other._y);
}

template <typename TValue> bool Vector2<TValue>::operator==(Vector2<TValue> const& other) const
{
    return _x == other._x && _y == other._y;
}

template <typename TValue> Vector2<TValue> Vector2<TValue>::operator*(double x)
{
    return Vector2<TValue>(_x * x, _y * x);
}

template <typename TValue> void Vector2<TValue>::rotate(const double theta)
{
    auto cs = cos(theta);
    auto sn = sin(theta);

    auto x = _x * cs - _y * sn;
    auto y = _x * sn + _y * cs;
    _x = x;
    _y = y;
}

template <typename TValue> void Vector2<TValue>::rotateDeg(const double theta)
{
    const auto PI = 3.14159265;
    const auto coeff = PI / 180.0;
    const auto rads = theta * coeff;
    rotate(rads);
}

template <typename T> std::ostream& operator<<(std::ostream& os, Vector2<T> const& v)
{
    os << v._x << " " << v._y;
    return os;
}

} // namespace Mino
