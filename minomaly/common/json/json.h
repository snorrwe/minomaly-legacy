#pragma once
#include "p_json_utils.h"

namespace Mino::Json
{
template <typename Class, typename T>
constexpr auto property(T Class::*member, const char* name)
{
    return Private::PropertyImpl<Class, T>{member, name};
}

/**
 *   Parse value T
 *   T has to be default contructable
 *   and type T must have a static member function named 'jsonProperties'
 *   that returns a tuple of the the json properties to be parsed.
 *   Parsed properties must be able to be set by the parse method
 *   (declare them as public)
 */
template <typename T, typename FwIt>
T parse(FwIt begin, FwIt end)
{
    static_assert(Private::IsJsonParseble<T>::value,
                  "Type must specify 'jsonProperties' static member "
                  "function to be used in this context!");
    auto parser = Private::ParseImpl<FwIt>{};
    return parser.template parse<T>(Private::Type<T>{}, begin, end);
}
}
