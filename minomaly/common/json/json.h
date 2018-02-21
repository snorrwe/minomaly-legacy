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
 *   that returns the json properties to be parsed
 *   parsed properties must be able to beset by the parse method
 *   (declare them as public)
 */
template <typename T, typename FwIt>
T parse(FwIt begin, FwIt end)
{
    auto parser = Private::ParseImpl<FwIt>{};
    return parser.parse<T>(Private::Type<T>{}, begin, end);
}
}
