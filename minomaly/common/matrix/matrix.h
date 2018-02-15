#pragma once
#include <array>
#include <cassert>
#include <iostream>

namespace Mino
{
template <uint8_t _cols, uint8_t _rows>
class Matrix
{
    std::array<float, (_cols * _rows)> values = {};

public:
    using TCol = uint8_t;

    constexpr Matrix() = default;
    constexpr Matrix(std::array<float, (_cols * _rows)> const& values)
        : values(values)
    {
    }
    constexpr Matrix(std::array<float, (_cols * _rows)>&& values)
        : values(std::move(values))
    {
    }
    constexpr Matrix(Matrix const&) = default;
    constexpr Matrix(Matrix&&) = default;
    ~Matrix() {}

    constexpr Matrix& operator=(Matrix const&) = default;
    constexpr Matrix& operator=(Matrix&&) = default;

    constexpr TCol columns() const { return _cols; }
    constexpr TCol rows() const { return _rows; }
    constexpr size_t size() const { return _cols * _rows; }

    constexpr float& at(TCol const col, TCol const row);
    constexpr float const& at(TCol const col, TCol const row) const;
    constexpr Matrix<_rows, _cols> transpose() const;

    template <TCol _c, TCol _r>
    constexpr Matrix<_cols, _r> operator*(Matrix<_c, _r> const&);
    constexpr Matrix<_cols, _rows> operator+(Matrix<_cols, _rows> const&);
    constexpr Matrix<_cols, _rows> operator-(Matrix<_cols, _rows> const&);

    template <TCol _c, TCol _r>
    constexpr bool operator==(Matrix<_c, _r> const&) const;
};

template <uint8_t _cols, uint8_t _rows>
std::ostream& operator<<(std::ostream& os, Matrix<_cols, _rows> const& m)
{
    os << "Matrix " << '0' + _cols << " " << '0' + _rows << "\n";
    for (int j = 0; j < _rows; ++j)
    {
        os << "| ";
        for (int i = 0; i < _cols; ++i)
        {
            os << m.at(i, j) << " |";
        }
        os << "\n";
    }
    return os;
}

template <uint8_t _cols, uint8_t _rows>
constexpr float& Matrix<_cols, _rows>::at(TCol const col, TCol const row)
{
    assert(col < _cols && row < _rows);
    return values[col + _cols * row];
}

template <uint8_t _cols, uint8_t _rows>
constexpr float const& Matrix<_cols, _rows>::at(TCol const col, TCol const row) const
{
    assert(col < _cols && row < _rows);
    return values[col + _cols * row];
}

template <uint8_t _cols, uint8_t _rows>
template <uint8_t _c, uint8_t _r>
constexpr Matrix<_cols, _r> Matrix<_cols, _rows>::operator*(Matrix<_c, _r> const& rhs)
{
    static_assert(_rows == _c);
    auto result = Matrix<_cols, _r>{};
    for (int col = 0; col < result.columns(); ++col)
    {
        for (int row = 0; row < result.rows(); ++row)
        {
            auto& value = result.at(col, row);
            for (int k = 0; k < rows(); ++k)
            {
                value += at(col, k) * rhs.at(k, row);
            }
        }
    }
    return result;
}

template <uint8_t _cols, uint8_t _rows>
template <uint8_t _c, uint8_t _r>
constexpr bool Matrix<_cols, _rows>::operator==(Matrix<_c, _r> const& rhs) const
{
    auto valuesAreEqual = [&]() {
        auto rit = rhs.values.begin();
        for (auto i = values.begin(); i != values.end(); ++i, ++rit)
        {
            if (*i != *rit) return false;
        }
        return true;
    };
    return _cols == _c && _rows == _r && valuesAreEqual();
}

template <uint8_t _cols, uint8_t _rows>
constexpr Matrix<_cols, _rows> Matrix<_cols, _rows>::operator+(Matrix<_cols, _rows> const& rhs)
{
    auto result = Matrix<_cols, _rows>{};
    auto rit = rhs.values.begin();
    auto resultIt = result.values.begin();
    for (auto i = values.begin(); i != values.end(); ++i, ++rit, ++resultIt)
    {
        *resultIt = *i + *rit;
    }
    return result;
}

template <uint8_t _cols, uint8_t _rows>
constexpr Matrix<_cols, _rows> Matrix<_cols, _rows>::operator-(Matrix<_cols, _rows> const& rhs)
{
    auto result = Matrix<_cols, _rows>{};
    auto rit = rhs.values.begin();
    auto resultIt = result.values.begin();
    for (auto i = values.begin(); i != values.end(); ++i, ++rit, ++resultIt)
    {
        *resultIt = *i - *rit;
    }
    return result;
}

template <uint8_t _cols, uint8_t _rows>
constexpr Matrix<_rows, _cols> Matrix<_cols, _rows>::transpose() const
{
    auto result = Matrix<_rows, _cols>{};
    for (int col = 0; col < columns(); ++col)
    {
        for (int row = 0; row < rows(); ++row)
        {
            result.at(row, col) = at(col, row);
        }
    }
    return result;
}

} // namespace Mino
