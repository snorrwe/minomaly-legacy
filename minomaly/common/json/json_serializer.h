#pragma once
#include "p_json_serializer.h"
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>

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

struct Object;
struct AnyJsonValue
{
    int integer = 0;
    double decimal = 0.0;
    std::string string = "";
    std::shared_ptr<Object> object = nullptr;
};
struct Object : std::map<std::string, AnyJsonValue>
{
    using std::map<std::string, AnyJsonValue>::operator[];

    AnyJsonValue const& operator[](std::string const& key) const
    {
        auto result = find(key);
        if (result != end()) return result->second;
        throw ParseError("[" + key + "]  does not exist in read json object!");
    }
};

template <typename T>
inline auto asAny(AnyJsonValue const& value)
{
    // TODO: support arbitrary objects
    throw std::domain_error("May not be called without specialization!");
}

template <>
inline auto asAny<int>(AnyJsonValue const& value)
{
    return value.integer;
}

template <>
inline auto asAny<double>(AnyJsonValue const& value)
{
    return value.decimal;
}

template <>
inline auto asAny<std::string>(AnyJsonValue const& value)
{
    return value.string;
}

using Private::property;

class JsonParser
{
public:
    enum class State
    {
        Default,
        Object,
        Key,
        UnknownValue,
        StringValue,
        NumberValue,
        DecimalValue,
        BoolValue, // TODO
        Array      // TODO
    };

    static bool isNumber(char chr) { return '0' <= chr && chr <= '9'; }

    static bool isWhiteSpace(char chr)
    {
        return chr == ' ' || chr == '\n' || chr == '\t' || chr == '\r';
    }

    static bool isLetter(char chr)
    {
        return ('a' <= chr && chr <= 'z') || ('A' <= chr && chr <= 'Z');
    }

    Object parse(std::string const& str);

private:
    void handleDefault(char chr, std::string& currentKey, std::stringstream& buffer);
    void handleObject(char chr, std::string& currentKey, std::stringstream& buffer);
    void handleKey(char chr, std::string& currentKey, std::stringstream& buffer);
    void handleUnknownValue(char chr, std::string& currentKey, std::stringstream& buffer);
    void handleNumberValue(char chr, std::string& currentKey, std::stringstream& buffer,
                           Object& result);
    void handleDecimalValue(char chr, std::string& currentKey, std::stringstream& buffer,
                            Object& result);
    void handleStringValue(char chr, std::string& currentKey, std::stringstream& buffer,
                           Object& result);

    State state = State::Default;
};

template <typename TData>
auto parse(std::string const& raw)
{
    auto data = JsonParser{}.parse(raw);
    return Private::fromJson<TData>(data);
}

} // namespace Mino::Json
