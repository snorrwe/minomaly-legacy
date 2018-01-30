#pragma once
#include <exception>
#include <iostream>
#include <memory>
#include <string>

namespace Mino::MatrixErrors
{

class MatrixError : public std::runtime_error
{
public:
    MatrixError(std::string message) : std::runtime_error(message) {}
    virtual ~MatrixError() {}

    MatrixError(MatrixError const&) = default;
    MatrixError(MatrixError&&) = default;
    MatrixError& operator=(MatrixError const&) = default;
    MatrixError& operator=(MatrixError&&) = default;

    virtual const char* what() const noexcept { return message->c_str(); }

protected:
    std::shared_ptr<std::string> message;
};

class InvalidMatrixPair : public MatrixError
{
public:
    InvalidMatrixPair() : MatrixError("InvalidMatrixPair") {}
    InvalidMatrixPair(int col1, int row1, int col2, int row2, std::string method) : MatrixError("")
    {
        auto str = std::string("InvalidMatrixPair [");
        str += std::to_string(col1);
        str += "×";
        str += std::to_string(row1);
        str += method;
        str += std::to_string(col2);
        str += "×";
        str += std::to_string(row2);
        str += "]";
        message = std::make_shared<std::string>(str);
    }

    InvalidMatrixPair(InvalidMatrixPair const&) = default;
    InvalidMatrixPair(InvalidMatrixPair&&) = default;
    InvalidMatrixPair& operator=(InvalidMatrixPair const&) = default;
    InvalidMatrixPair& operator=(InvalidMatrixPair&&) = default;
};

class InvalidAccess : public MatrixError
{
public:
    InvalidAccess() : MatrixError("InvalidAccess") {}
    InvalidAccess(int col1, int row1) : MatrixError("")
    {
        std::string str("InvalidAccess [");
        str += std::to_string(col1);
        str += "×";
        str += std::to_string(row1);
        str += "]";
        message = std::make_unique<std::string>(str);
    }

    InvalidAccess(InvalidAccess const&) = default;
    InvalidAccess(InvalidAccess&&) = default;
    InvalidAccess& operator=(InvalidAccess const&) = default;
    InvalidAccess& operator=(InvalidAccess&&) = default;
};

class InvalidColumnAccess : public MatrixError
{
public:
    InvalidColumnAccess() : MatrixError("InvalidColumnAccess") {}
    InvalidColumnAccess(int col1) : MatrixError("")
    {
        std::string str("InvalidColumnAccess [");
        str += std::to_string(col1);
        str += "]";
        message = std::make_unique<std::string>(str);
    }

    InvalidColumnAccess(InvalidColumnAccess const&) = default;
    InvalidColumnAccess(InvalidColumnAccess&&) = default;
    InvalidColumnAccess& operator=(InvalidColumnAccess const&) = default;
    InvalidColumnAccess& operator=(InvalidColumnAccess&&) = default;
};

class BadSize : public MatrixError
{
public:
    BadSize() : MatrixError("BadSize") {}
    BadSize(std::string const& message) : MatrixError("")
    {
        std::string str("BadSize [");
        str += message;
        str += "]";
        this->message = std::make_unique<std::string>(str);
    }

    BadSize(BadSize const&) = default;
    BadSize(BadSize&&) = default;
    BadSize& operator=(BadSize const&) = default;
    BadSize& operator=(BadSize&&) = default;
};
}
