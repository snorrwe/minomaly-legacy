#include "matrix.h"

using namespace Mino;
using namespace Mino::MatrixErrors;
using Matrix = Mino::Matrix;

Matrix Mino::operator*(float const& coeff, Matrix result) { return result *= coeff; }

Matrix Mino::operator*(Matrix result, float const& coeff) { return result *= coeff; }

std::vector<float> Matrix::invertValues(Matrix const& matrix)
{
    std::vector<float> rightValues(matrix.size());
    for (int i = 0; i != matrix.size(); ++i)
    {
        int y = i % matrix.rows();
        int x = (i - y) / matrix.rows();
        rightValues[y * matrix.columns() + x] = matrix.values[i];
    }
    return rightValues;
}

Matrix Mino::operator*(Matrix const& lhs, Matrix const& rhs)
{
    if (lhs.rows() != rhs.columns())
    {
        throw MatrixErrors::InvalidMatrixPair(lhs.columns(), lhs.rows(), rhs.columns(), rhs.rows(),
                                              " dot ");
    }
    auto rightValues = Matrix::invertValues(rhs);
    Matrix result(lhs.columns(), rhs.rows());
    for (int col = 0; col < result.columns(); ++col)
    {
        for (int row = 0; row < result.rows(); ++row)
        {
            auto& value = result.get(col, row);
            for (int k = 0; k < lhs.rows(); ++k)
            {
                value += lhs.get(col, k) * rightValues[row * rhs.columns() + k];
            }
        }
    }
    return result;
}

Matrix Mino::operator+(Matrix lhs, Matrix const& rhs) { return lhs += rhs; }

Matrix Mino::operator-(Matrix lhs, Matrix const& rhs) { return lhs -= rhs; }

std::ostream& Mino::operator<<(std::ostream& stream, Matrix const& matrix)
{
    stream << "Matrix: [" << matrix.columns() << "×" << matrix.rows() << "]\n";
    for (int j = 0; j < matrix.rows(); ++j)
    {
        stream << "|";
        for (int i = 0; i < matrix.columns(); ++i)
        {
            stream << " " << matrix.get(i, j) << " |";
        }
        stream << "\n";
    }
    return stream;
}

Matrix::Matrix(std::vector<std::vector<float>> const& matrix)
    : _columns(matrix.size()), _rows(matrix[0].size()), values(_columns * _rows)
{
    for (int i = 0; i < _columns; ++i)
    {
        for (int j = 0; j < _rows; ++j)
        {
            set(i, j, matrix[i][j]);
        }
    }
}

Matrix::Matrix(int columns, int rows) : _columns(columns), _rows(rows), values(columns * rows)
{
    values.shrink_to_fit();
}

Matrix::Matrix(std::vector<float> const& matrix, int columns, int rows)
    : _columns(columns), _rows(rows), values(columns * rows)
{
    if (matrix.size() != columns * rows)
    {
        throw std::runtime_error("Input vector must have a size of columns×rows!");
    }
    for (int i = 0; i < columns; ++i)
    {
        for (int j = 0; j < rows; ++j)
        {
            auto index = i + (columns * j);
            set(i, j, matrix[index]);
        }
    }
    values.shrink_to_fit();
}

Matrix::ColumnProxy Matrix::operator[](int n)
{
    auto col = n * _rows;
    if (col > _columns * _rows) throw MatrixErrors::InvalidColumnAccess(n);
    return ColumnProxy(col, _rows, values);
}

void Matrix::set(int col, int row, float value)
{
    if (col > _columns || row > _rows) throw MatrixErrors::InvalidAccess(col, row);
    values[col * _rows + row] = value;
}

float& Matrix::get(int col, int row)
{
    if (col > _columns || row > _rows) throw MatrixErrors::InvalidAccess(col, row);
    return values[col * _rows + row];
}

float const& Matrix::get(int col, int row) const
{
    if (col > _columns || row > _rows) throw MatrixErrors::InvalidAccess(col, row);
    return values[col * _rows + row];
}

bool Matrix::operator==(Matrix const& rhs) const
{
    if (columns() != rhs.columns() || rows() != rhs.rows()) return false;
    auto rhi = rhs.begin();
    for (auto i = begin(); i != end(); ++i, ++rhi)
    {
        if (*i != *rhi) return false;
    }
    return true;
}

Matrix& Matrix::operator*=(float const& coeff)
{
    for (auto i = begin(); i != end(); ++i)
    {
        *i = coeff * (*i);
    }
    return *this;
}

Matrix Matrix::transpose(Matrix const& matrix)
{
    Matrix result(matrix.rows(), matrix.columns());
    for (int col = 0; col < matrix.columns(); ++col)
    {
        for (int row = 0; row < matrix.rows(); ++row)
        {
            result[row][col] = matrix.get(col, row);
        }
    }
    return result;
}

Matrix Matrix::uniform(int columns, int rows, float value)
{
    Matrix result(columns, rows);
    for (auto i = result.begin(); i != result.end(); ++i)
    {
        *i = value;
    }
    return result;
}

Matrix Matrix::map(Matrix const& lhs, Matrix const& rhs,
                   std::function<float(float, float)> callback)
{
    if (lhs._columns != rhs._columns || lhs._rows != rhs._rows)
    {
        throw MatrixErrors::InvalidMatrixPair(lhs._columns, lhs._rows, rhs._columns, rhs._rows,
                                              " map ");
    }
    Matrix result{lhs._columns, lhs._rows};
    auto ri = rhs.begin();
    auto resultI = result.begin();
    for (auto li = lhs.begin(); li != lhs.end(); ++li, ++ri, ++resultI)
    {
        *resultI = callback(*li, *ri);
    }
    return result;
}

Matrix Matrix::hadamard(Matrix const& lhs, Matrix const& rhs)
{
    Matrix::assertEqualSize(lhs, rhs, " hadamard ");
    return Matrix::map(lhs, rhs, [](float l, float r) { return l * r; });
}

void Matrix::assertEqualSize(Matrix const& lhs, Matrix const& rhs, std::string const& message)
{
    if (lhs._columns != rhs._columns || lhs._rows != rhs._rows)
    {
        throw MatrixErrors::InvalidMatrixPair(lhs._columns, lhs._rows, rhs._columns, rhs._rows,
                                              message);
    }
}

void Matrix::add(Matrix const& matrix, short coeff)
{
    Matrix::assertEqualSize(*this, matrix, coeff > 0 ? " + " : " - ");
    if (_columns == 0 || _rows == 0) return;
    auto mi = matrix.begin();
    for (auto i = begin(); i != end(); ++i, ++mi)
    {
        *i += coeff * (*mi);
    }
}

Matrix& Matrix::operator+=(Matrix const& matrix)
{
    add(matrix, 1);
    return *this;
}

Matrix& Matrix::operator-=(Matrix const& matrix)
{
    add(matrix, -1);
    return *this;
}

Matrix::ColumnProxy::operator std::vector<float>()
{
    std::vector<float> result{};
    for (int i = 0; i < _size; ++i)
    {
        result.push_back(values[i + begin]);
    }
    return result;
}

float Matrix::length() const
{
    if (_columns == 0 && _rows == 0) return 0.0;
    if (_columns != 1 && _rows != 1)
    {
        throw MatrixErrors::BadSize(
            "Length is only defined on vectors (Either the columns or rows has to be 1)");
    }
    float sum = 0.0;
    for (auto i = this->begin(); i != this->end(); ++i)
    {
        sum += (*i) * (*i);
    }
    return sqrt(sum);
}
