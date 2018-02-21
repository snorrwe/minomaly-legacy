#pragma once
#include "p_json_error.h"
#include <sstream>
#include <string>

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

template <typename FwIt>
struct ParseImpl
{
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
        /*for (; begin != end and *begin != ','; ++begin)
        {
            stream
        }*/
        return 0; // TODO
    }

    ParseState state = ParseState::Default;
};
}
