#pragma once
#include "p_json_error.h"
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

namespace Mino::Json::Private
{
template <typename Class, typename T>
struct PropertyImpl
{
    constexpr PropertyImpl(T Class::*aMember, const char* aName)
        : member{aMember}
        , name{aName}
    {
    }

    using Type = T;

    T Class::*member;
    const char* name;
};

template <typename T>
struct Type
{
};

// sequence for
template <typename T, T... S, typename F>
constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f)
{
    using unpack_t = int[];
    (void)unpack_t{(static_cast<void>(f(std::integral_constant<T, S>{})), 0)..., 0};
}

constexpr auto strEqual(const char* lhs, const char* rhs)
{
    for (; *lhs && *rhs; ++lhs, ++rhs)
    {
        if (*lhs != *rhs) return false;
    }
    return *lhs == *rhs;
}

template <typename T, typename Fun>
constexpr void executeByPropertyName(const char* name, Fun&& f)
{
    constexpr auto nbProperties = std::tuple_size<decltype(T::jsonProperties())>::value;
    for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i) {
        constexpr auto property = std::get<i>(T::jsonProperties());
        if (strEqual(property.name, name))
        {
            f(property);
        }
    });
}

template <typename TResult, typename TValue>
constexpr void setProperty(TResult& result, const char* name, TValue value)
{
    executeByPropertyName<TResult>(
        name, [&](auto property) { (TValue&)(result.*(property.member)) = value; });
};

template <typename T>
class IsJsonParseble
{
    struct Two
    {
        char _[2];
    };

    typedef char one;
    typedef Two two;

    template <typename C>
    static one test(char[&C::jsonProperties]);
    template <typename C>
    static two test(...);

public:
    enum
    {
        value = sizeof(test<T>(0)) == sizeof(char)
    };
};

template <typename FwIt>
class ParseImpl
{
public:
    enum ParseState
    {
        Default,
        Key,
        Value
    };

    template <typename T>
    T parse(Type<T>, FwIt& begin, FwIt end)
    {
        static_assert(Private::IsJsonParseble<T>::value,
                      "Type must specify 'jsonProperties' static member "
                      "function to be used in this context!");
        auto result = T{};
        state = ParseState::Default;
        auto key = ""s;
        skipUntil(begin, end, [](auto c) { return !isWhiteSpace(c); });
        if (*begin != '{')
        {
            throwUnexpectedCharacter(*begin);
        }
        ++begin;
        while (begin != end)
        {
            switch (state)
            {
            case ParseState::Default:
                if (isWhiteSpace(*begin))
                {
                    continue;
                }
                if (*begin == '"')
                {
                    state = ParseState::Key;
                }
                else
                {
                    throwUnexpectedCharacter(*begin);
                }
                break;
            case ParseState::Key:
                if (*begin == '"')
                {
                    state = ParseState::Value;
                    ++begin;
                    skipUntil(begin, end, [](auto c) { return !isWhiteSpace(c); });
                    if (*begin != ':')
                    {
                        throwUnexpectedCharacter(*begin);
                    }
                    ++begin;
                }
                else
                {
                    key += *begin;
                }
                break;
            case ParseState::Value:
                executeByPropertyName<T>(key.c_str(), [&](auto property) {
                    setProperty(result, key.c_str(),
                                parseValue<decltype(property)::Type>(begin, end));
                });
                state = ParseState::Default;
                key.clear();
                skipUntil(begin, end, [](auto c) { return !isWhiteSpace(c) && !isValueEnd(c); });
                continue;
            }
            ++begin;
        }
        if (state != ParseState::Default)
        {
            throw ParseError("Unexpected end to the json input!");
        }
        return result;
    }

    int parse(Type<int>, FwIt& begin, FwIt end) { return parseNumber<int>(begin, end); }
    double parse(Type<double>, FwIt& begin, FwIt end) { return parseNumber<double>(begin, end); }

    std::string parse(Type<std::string>, FwIt& begin, FwIt end)
    {
        skipUntil(begin, end, [](auto c) { return !isWhiteSpace(c); });
        if (*begin == '"')
        {
            ++begin;
        }
        else
        {
            throwUnexpectedCharacter(*begin);
        }
        auto stream = std::stringstream{};
        for (; begin != end && *begin != '"'; ++begin)
        {
            stream << *begin;
        }
        if (begin == end)
        {
            throw ParseError("Unexpected end to the json input!");
        }
        ++begin;
        return stream.str();
    }

private:
    static bool isNumber(char c) { return '0' <= c && c <= '9'; }
    static bool isWhiteSpace(char c) { return c == ' ' || c == '\t' || c == '\n'; }
    static bool isValueEnd(char c) { return c == ',' || c == '}'; }

    template <typename TResult>
    TResult parseNumber(FwIt& begin, FwIt end)
    {
        skipUntil(begin, end, [](auto c) { return !isWhiteSpace(c); });
        auto stream = std::stringstream{};
        stream << *begin++;
        for (; begin != end && (isNumber(*begin) && !isValueEnd(*begin) && !isWhiteSpace(*begin));
             ++begin)
        {
            stream << *begin;
        }
        TResult result;
        stream >> result;
        return result;
    }

    template <typename TResult>
    TResult parseValue(FwIt& begin, FwIt end)
    {
        return ParseImpl<FwIt>{}.parse(Type<TResult>{}, begin, end);
    }

    void throwUnexpectedCharacter(char chr)
    {
        throw ParseError("Unexpected character: ["s + chr + "] in json input!");
    }

    template <typename Fun>
    void skipUntil(FwIt& begin, FwIt end, Fun&& predicate)
    {
        auto result = begin == end || predicate(*begin);
        while (!result)
        {
            ++begin;
            result = begin == end || predicate(*begin);
        }
        if (begin == end && !result)
        {
            throw ParseError("Unexpected end to the json input!");
        }
    }

    ParseState state = ParseState::Default;
};
}