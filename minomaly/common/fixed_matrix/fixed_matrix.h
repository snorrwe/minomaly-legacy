#pragma once
#include <array>
#include <cassert>

namespace Mino
{

template <uint8_t _cols, uint8_t _rows> class FixedMatrix
{
    std::array<float, (_cols * _rows)> values = {};

public:
    using TCol = uint8_t;

    FixedMatrix() = default;
    FixedMatrix(std::array<float, (_cols * _rows)> const& values) : values(values) {}
    FixedMatrix(std::array<float, (_cols * _rows)>&& values) : values(std::move(values)) {}
    FixedMatrix(FixedMatrix const&) = default;
    FixedMatrix(FixedMatrix&&) = default;
    ~FixedMatrix() {}

    FixedMatrix& operator=(FixedMatrix const&) = default;
    FixedMatrix& operator=(FixedMatrix&&) = default;

    constexpr TCol columns() const { return _cols; }
    constexpr TCol rows() const { return _rows; }
    constexpr size_t size() { return _cols * _rows; }

    float& at(TCol const col, TCol const row);
    float const& at(TCol const col, TCol const row) const;
    FixedMatrix<_rows, _cols> transpose() const;

    template <TCol _c, TCol _r> FixedMatrix<_cols, _r> operator*(FixedMatrix<_c, _r> const&);
    FixedMatrix<_cols, _cols> operator+(FixedMatrix<_cols, _rows> const&);
    FixedMatrix<_cols, _cols> operator-(FixedMatrix<_cols, _rows> const&);

    template <TCol _c, TCol _r> bool operator==(FixedMatrix<_c, _r> const&) const;
};

template <uint8_t _cols, uint8_t _rows>
float& FixedMatrix<_cols, _rows>::at(TCol const col, TCol const row)
{
    assert(col < _cols && row < _rows);
    return values[col * _rows + row];
}

template <uint8_t _cols, uint8_t _rows>
float const& FixedMatrix<_cols, _rows>::at(TCol const col, TCol const row) const
{
    assert(col < _cols && row < _rows);
    return values[col * _rows + row];
}

template <uint8_t _cols, uint8_t _rows>
template <uint8_t _c, uint8_t _r>
FixedMatrix<_cols, _r> FixedMatrix<_cols, _rows>::operator*(FixedMatrix<_c, _r> const& rhs)
{
    static_assert(_rows == _c);
    auto result = FixedMatrix<_cols, _r>{};
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
bool FixedMatrix<_cols, _rows>::operator==(FixedMatrix<_c, _r> const& rhs) const
{
    return _cols == _c && _rows == _r && [&]() {
        auto rit = rhs.values.begin();
        for (auto i = values.begin(); i != values.end(); ++i, ++rit)
        {
            if (*i != *rit) return false;
        }
        return true;
    }();
}

template <uint8_t _cols, uint8_t _rows>
FixedMatrix<_cols, _cols> FixedMatrix<_cols, _rows>::operator+(FixedMatrix<_cols, _rows> const& rhs)
{
    auto result = FixedMatrix<_cols, _rows>{};
    auto rit = rhs.values.begin();
    auto resultIt = result.values.begin();
    for (auto i = values.begin(); i != values.end(); ++i, ++rit, ++resultIt)
    {
        *resultIt = *i + *rit;
    }
    return result;
}

template <uint8_t _cols, uint8_t _rows>
FixedMatrix<_cols, _cols> FixedMatrix<_cols, _rows>::operator-(FixedMatrix<_cols, _rows> const& rhs)
{
    auto result = FixedMatrix<_cols, _rows>{};
    auto rit = rhs.values.begin();
    auto resultIt = result.values.begin();
    for (auto i = values.begin(); i != values.end(); ++i, ++rit, ++resultIt)
    {
        *resultIt = *i - *rit;
    }
    return result;
}

template <uint8_t _cols, uint8_t _rows>
FixedMatrix<_rows, _cols> FixedMatrix<_cols, _rows>::transpose() const
{
    auto result = FixedMatrix<_rows, _cols>{};
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
