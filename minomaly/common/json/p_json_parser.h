#pragma once
#include "p_json_error.h"
#include "p_json_utility.h"
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace Mino::Json::Private
{
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
                skipUntil(begin, end, [](auto c) { return !isWhiteSpace(c); });
                if (*begin == '"')
                {
                    state = ParseState::Key;
                }
                else if (*begin == '}')
                {
                    ++begin;
                    return result;
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
                }
                else
                {
                    key += *begin;
                }
                break;
            case ParseState::Value:
                executeByPropertyName<T>(key.c_str(), [&](auto property) {
                    using PropertyType = typename decltype(property)::Type;
                    setProperty(result, key.c_str(), parseValue<PropertyType>(begin, end));
                });
                state = ParseState::Default;
                key.clear();
                skipUntil(begin, end, [](auto c) { return !isWhiteSpace(c) && !isValueEnd(c); });
                continue;
            }
            ++begin;
        }
        throw ParseError("Unexpected end to the json input!");
    }

    template <typename T>
    std::vector<T> parse(Type<std::vector<T>>, FwIt& begin, FwIt end)
    {
        skipUntil(begin, end, [](auto c) { return !isWhiteSpace(c); });
        if (*begin != '[')
        {
            throwUnexpectedCharacter(*begin);
        }
        ++begin;
        auto result = std::vector<T>{};
        while (*begin != ']')
        {
            result.push_back(parse(Type<T>{}, begin, end));
            skipUntil(begin, end, [](auto c) { return !isWhiteSpace(c) && !isValueEnd(c); });
        }
        ++begin;
        return result;
    }

    int parse(Type<int>, FwIt& begin, FwIt end)
    {
        skipUntil(begin, end, [](auto c) { return !isWhiteSpace(c); });
        auto stream = std::stringstream{};
        stream << *begin++;
        if (isNumber(*begin))
        {
            stream << parse(Type<unsigned>{}, begin, end);
        }
        int result = 0;
        stream >> result;
        return result;
    }

    unsigned parse(Type<unsigned>, FwIt& begin, FwIt end)
    {
        skipUntil(begin, end, [](auto c) { return !isWhiteSpace(c); });
        auto stream = std::stringstream{};
        while (begin != end && (isNumber(*begin) && !isValueEnd(*begin) && !isWhiteSpace(*begin)))
        {
            stream << *begin;
            ++begin;
        }
        unsigned result = 0;
        stream >> result;
        return result;
    }

    float parse(Type<float>, FwIt& begin, FwIt end) { return parseFloat<float>(begin, end); }
    double parse(Type<double>, FwIt& begin, FwIt end) { return parseFloat<double>(begin, end); }

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
    static bool isValueEnd(char c) { return c == ','; }

    template <typename TResult>
    TResult parseFloat(FwIt& begin, FwIt end)
    {
        skipUntil(begin, end, [](auto c) { return !isWhiteSpace(c); });
        auto stream = std::stringstream{};
        stream << *begin++;
        while (begin != end
               && ((isNumber(*begin) || *begin == '.') && !isValueEnd(*begin)
                   && !isWhiteSpace(*begin)))
        {
            stream << *begin;
            ++begin;
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
