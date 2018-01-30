#pragma once
#include "matrix.errors.h"
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

namespace Mino
{

class Matrix;

Matrix operator*(float const& coeff, Matrix rhs);
Matrix operator*(Matrix lhs, float const& coeff);
Matrix operator*(Matrix const& lhs, Matrix const& rhs);
Matrix operator+(Matrix lhs, Matrix const& rhs);
Matrix operator-(Matrix lhs, Matrix const& rhs);
std::ostream& operator<<(std::ostream& stream, Matrix const& matrix);

class Matrix
{
    int _columns = 0;
    int _rows = 0;
    std::vector<float> values = {};

public:
    using iterator = std::vector<float>::iterator;
    using const_iterator = std::vector<float>::const_iterator;

    class ColumnProxy
    {
    public:
        ColumnProxy(int begin, int size, std::vector<float>& values)
            : begin(begin), _size(size), values(values)
        {
        }
        ColumnProxy(ColumnProxy const& other) : begin(other.begin), values(other.values) {}
        ~ColumnProxy() {}

        float& operator[](int n)
        {
            if (n > _size) throw MatrixErrors::InvalidAccess(begin, n);
            return values[begin + n];
        }
        operator std::vector<float>();
        int size() { return _size; }

    private:
        int begin;
        int _size;
        std::vector<float>& values;
    };

    static Matrix transpose(Matrix const& matrix);
    static Matrix uniform(int columns, int rows, float value = 1.0);
    static Matrix map(Matrix const& lhs, Matrix const& rhs,
                      std::function<float(float, float)> callback);
    static Matrix hadamard(Matrix const& lhs, Matrix const& rhs);
    static Matrix dot(Matrix const& lhs, Matrix const& rhs) { return lhs * rhs; }
    static void assertEqualSize(Matrix const& lhs, Matrix const& rhs, std::string const& message);
    static std::vector<float> invertValues(Matrix const& matrix);

    Matrix() = default;
    Matrix(int columns, int rows);
    Matrix(std::vector<std::vector<float>> const& matrix);
    Matrix(std::vector<float> const& matrix, int columns, int rows);
    Matrix(Matrix const& matrix) = default;
    Matrix(Matrix&& matrix) = default;
    ~Matrix() {}

    ColumnProxy operator[](int n);
    Matrix& operator=(Matrix const& matrix) = default;
    Matrix& operator=(Matrix&& matrix) = default;
    Matrix& operator*=(float const& coeff);
    Matrix& operator+=(Matrix const& matrix);
    Matrix& operator-=(Matrix const& matrix);

    bool operator==(Matrix const& rhs) const;

    void set(int col, int row, float value);
    float& get(int col, int row);
    float const& get(int col, int row) const;
    ColumnProxy const getColumn(int col) const;
    Matrix transpose() const { return Matrix::transpose(*this); }

    int columns() const { return _columns; }
    int rows() const { return _rows; }
    int size() const { return _columns * _rows; }
    float length() const;
    iterator begin() { return values.begin(); }
    const_iterator begin() const { return values.begin(); }
    iterator end() { return values.end(); }
    const_iterator end() const { return values.end(); }

private:
    void add(Matrix const& matrix, short coeff);
};

} // namespace Mino
