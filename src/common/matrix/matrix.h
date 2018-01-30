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

Matrix operator*(double const& coeff, Matrix rhs);
Matrix operator*(Matrix lhs, double const& coeff);
Matrix operator*(Matrix const& lhs, Matrix const& rhs);
Matrix operator+(Matrix lhs, Matrix const& rhs);
Matrix operator-(Matrix lhs, Matrix const& rhs);
std::ostream& operator<<(std::ostream& stream, Matrix const& matrix);

class Matrix
{
    int _columns = 0;
    int _rows = 0;
    std::vector<double> values = {};

public:
    using iterator = std::vector<double>::iterator;
    using const_iterator = std::vector<double>::const_iterator;

    class ColumnProxy
    {
    public:
        ColumnProxy(int begin, int size, std::vector<double>& values)
            : begin(begin), _size(size), values(values)
        {
        }
        ColumnProxy(ColumnProxy const& other) : begin(other.begin), values(other.values) {}
        ~ColumnProxy() {}

        double& operator[](int n)
        {
            if (n > _size) throw MatrixErrors::InvalidAccess(begin, n);
            return values[begin + n];
        }
        operator std::vector<double>();
        int size() { return _size; }

    private:
        int begin;
        int _size;
        std::vector<double>& values;
    };

    static Matrix transpose(Matrix const& matrix);
    static Matrix uniform(int columns, int rows, double value = 1.0);
    static Matrix map(Matrix const& lhs, Matrix const& rhs,
                      std::function<double(double, double)> callback);
    static Matrix hadamard(Matrix const& lhs, Matrix const& rhs);
    static Matrix dot(Matrix const& lhs, Matrix const& rhs) { return lhs * rhs; }
    static void assertEqualSize(Matrix const& lhs, Matrix const& rhs, std::string const& message);
    static std::vector<double> invertValues(Matrix const& matrix);

    Matrix() = default;
    Matrix(int columns, int rows);
    Matrix(std::vector<std::vector<double>> const& matrix);
    Matrix(std::vector<double> const& matrix, int columns, int rows);
    Matrix(Matrix const& matrix) = default;
    Matrix(Matrix&& matrix) = default;
    ~Matrix() {}

    ColumnProxy operator[](int n);
    Matrix& operator=(Matrix const& matrix) = default;
    Matrix& operator=(Matrix&& matrix) = default;
    Matrix& operator*=(double const& coeff);
    Matrix& operator+=(Matrix const& matrix);
    Matrix& operator-=(Matrix const& matrix);

    bool operator==(Matrix const& rhs) const;

    void set(int col, int row, double value);
    double& get(int col, int row);
    double const& get(int col, int row) const;
    ColumnProxy const getColumn(int col) const;
    Matrix transpose() const { return Matrix::transpose(*this); }

    int columns() const { return _columns; }
    int rows() const { return _rows; }
    int size() const { return _columns * _rows; }
    double length() const;
    iterator begin() { return values.begin(); }
    const_iterator begin() const { return values.begin(); }
    iterator end() { return values.end(); }
    const_iterator end() const { return values.end(); }

private:
    void add(Matrix const& matrix, short coeff);
};

} // namespace Mino
