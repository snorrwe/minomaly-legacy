#pragma once

namespace Mino
{

template <typename TValue = double> class Vector2
{
public:
    Vector2() = default;
    Vector2(TValue x, TValue y) : x(x), y(y) {}
    Vector2(Vector2 const&) = default;
    Vector2(Vector2&&) = default;
    ~Vector2() = default;

    Vector2& operator=(Vector2 const&) = default;
    Vector2& operator=(Vector2&&) = default;

    Vector2 operator+(Vector2 const& other);
    Vector2 operator-(Vector2 const& other);

    const TValue x = 0;
    const TValue y = 0;
};

template <typename TValue = double>
Vector2<TValue> Vector2<TValue>::operator+(Vector2<TValue> const& other)
{
    return Vector2<TValue>(x + other.x, y + other.y);
}

template <typename TValue = double>
Vector2<TValue> Vector2<TValue>::operator-(Vector2<TValue> const& other)
{
    return Vector2<TValue>(x - other.x, y - other.y);
}

} // namespace Mino
