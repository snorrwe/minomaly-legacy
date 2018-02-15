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
    static_assert(false, "May not be called without specialization!");
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

template <>
inline auto asAny<Object>(AnyJsonValue const& value)
{
    return *value.object;
}

using Private::property;

class JsonParser
{
    enum class State
    {
        Default,
        Object,
        Key,
        UnknownValue,
        StringValue,
        NumberValue,
        DecimalValue,
        // BoolValue,//TODO
        Array
    };

    State state = State::Default;

public:
    Object parse(std::string const& str)
    {
        auto result = Object{};

        auto* current = &result;
        State state = State::Default;

        auto sstr = std::istringstream{str};
        auto buffer = std::stringstream{};
        auto currentKey = std::string{};
        while (sstr)
        {
            char chr;
            sstr >> chr;

            switch (state)
            {
            case State::Default:
            {
                switch (chr)
                {
                case '{':
                    state = State::Object;
                    break;
                case '[':
                    state = State::Array;
                    break;
                }
                break;
            }
            case State::Object:
            {
                if (chr == '"')
                {
                    state = State::Key;
                }
                else if (!currentKey.empty() && chr == ':')
                {
                    state = State::UnknownValue;
                }
                else if (chr == '}')
                {
                    state = State::Default; // TODO: state.pop
                }
                else if (isWhiteSpace(chr) || chr == ',')
                {
                    break;
                }
                else
                {
                    throw ParseError(std::string{"Unexpected character: ["} + chr
                                     + "] in JSON string!");
                }
                break;
            }
            case State::Key:
            {
                if (chr == '"')
                {
                    state = State::Object;
                    buffer >> currentKey;
                    buffer = std::stringstream{};
                }
                else
                {
                    buffer << chr;
                }
                break;
            }
            case State::UnknownValue:
            {
                if (isNumber(chr) || chr == '-')
                {
                    buffer << chr;
                    state = State::NumberValue;
                }
                else if (chr == '"')
                {
                    state = State::StringValue;
                }
                else if (chr == '{')
                {
                    // TODO
                    throw std::runtime_error("Not implemented");
                }
                else if (chr == '[')
                {
                    // TODO
                    throw std::runtime_error("Not implemented");
                }
                else
                {
                    throw ParseError(std::string{"Unexpected character: ["} + chr
                                     + "] in JSON value string!");
                }
                break;
            }
            case State::NumberValue:
            {
                if (isNumber(chr))
                {
                    buffer << chr;
                }
                else if (chr == '.')
                {
                    state = State::DecimalValue;
                    buffer << chr;
                }
                else
                {
                    // TODO: assert next character for correctness
                    state = State::Object; // TODO: state.pop
                    buffer >> current->operator[](currentKey).integer;
                    buffer = std::stringstream{};
                    currentKey.clear();
                }
                break;
            }
            case State::DecimalValue:
            {
                if (isNumber(chr))
                {
                    buffer << chr;
                }
                else
                {
                    state = State::Object; // TODO: state.pop
                    buffer >> current->operator[](currentKey).decimal;
                    buffer = std::stringstream{};
                    currentKey.clear();
                }
                break;
            }
            case State::StringValue:
            {
                if (chr == '"')
                {
                    state = State::Object; // TODO: state.pop
                    buffer >> current->operator[](currentKey).string;
                    buffer = std::stringstream{};
                    currentKey.clear();
                }
                else
                {
                    buffer << chr;
                }
                break;
            }
            }
        }
        return result;
    }

private:
    bool isNumber(char chr) { return '0' <= chr && chr <= '9'; }
    bool isWhiteSpace(char chr) { return chr == ' ' || chr == '\n' || chr == '\t' || chr == '\r'; }
    bool isLetter(char chr) { return ('a' <= chr && chr <= 'z') || ('A' <= chr && chr <= 'Z'); }
};

template <typename TData>
auto parse(std::string const& raw)
{
    auto data = JsonParser{}.parse(raw);
    return Private::fromJson<TData>(data);
}

} // namespace Mino::Json
