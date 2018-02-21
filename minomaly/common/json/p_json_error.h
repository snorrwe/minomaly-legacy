#pragma once
#include <stdexcept>
#include <string>

namespace Mino::Json
{
struct ParseError : public std::exception
{
    std::string message;

    ParseError(std::string msg)
        : message(std::move(msg))
    {
    }
    ParseError(ParseError const&) = default;
    ParseError& operator=(ParseError const&) = default;

    const char* what() const override { return message.c_str(); }
};
}
