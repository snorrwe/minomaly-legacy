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

template <typename TResult, typename TValue>
void setProperty(TResult& result, const char* name, TValue value)
{
    constexpr auto nbProperties = std::tuple_size<decltype(TResult::jsonProperties())>::value;
    for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i) {
        constexpr auto property = std::get<i>(TResult::jsonProperties());
        if (strEqual(property.name, name))
        {
            (TValue&)(result.*(property.member)) = value;
        }
    });
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
    T parse(Type<T>, FwIt begin, FwIt end)
    {
        auto result = T{};
        // TODO: create property pointer / name / type lookup table
        state = ParseState::Default;
        for (; begin != end; ++begin)
        {
            switch (state)
            {
            case ParseState::Default:
                break;
            case ParseState::Key:
                break;
            case ParseState::Value:
                break;
            }
        }
        return result;
    }

    int parse(Type<int>, FwIt begin, FwIt end)
    {
        auto stream = std::stringstream{};
        /*
        for (; begin != end and *begin != ','; ++begin)
        {
            stream
        }
        */
        return 0; // TODO
    }

private:
    ParseState state = ParseState::Default;
};
}
