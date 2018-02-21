#pragma once
#include "p_json_error.h"
#include "p_json_utils.h"
#include <sstream>
#include <string>

namespace Mino::Json
{
template <typename Class, typename T>
constexpr auto property(T Class::*member, const char* name)
{
    return Private::PropertyImpl<Class, T>{member, name};
}

template <typename FwIt>
struct Parser
{
    enum ParseState
    {
        Default,
        Key,
        Value
    };

    /**
     *   Parse value T
     *   T has to be default contructable
     *   and type T must have a static member function named 'jsonProperties'
     *   that returns the json properties to be parsed
     *   parsed properties must be able to beset by the parse method
     *   (declare them as public)
     */
    template <typename T>
    T parse(FwIt begin, FwIt end)
    {
        auto result = T{};
        // TODO: create property pointer / name / type lookup table
        auto state = ParseState::Default;
        for (; begin != end; ++begin)
        {
            switch (state)
            {
            case ParseState::Default:
                break;
            case ParseState::Key:
                break;
            case ParseState::Value:
                auto valueStream = std::stringstream{};
                // TODO ...
                // get Type from the property table
                Private::parseValue<int>(valueStream);
                break;
            }
        }

        return result;
    }
};

template <typename T, typename FwIt>
T parse(FwIt begin, FwIt end)
{
    auto parser = Parser<FwIt>{};
    return parser.parse<T>(begin, end);
}
}
