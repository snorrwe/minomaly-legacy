#pragma once
#include "p_json_parser.h"
#include "p_json_serializer.h"
#include <iostream>
#include <iterator>
#include <string>

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
    auto parser = Private::ParseImpl<FwIt>{begin, end};
    return parser.template parse<T>(Private::Type<T>{});
}

template <typename T>
T parse(std::istream& stream)
{
    return parse<T>(std::istream_iterator<char>(stream), std::istream_iterator<char>());
}

template <typename T, typename TStream>
void serialize(T const& item, TStream& result)
{
    static_assert(Private::IsJsonParseble<T>::value,
                  "Type must specify 'jsonProperties' static member "
                  "function to be used in this context!");
    auto serializer = Private::SerializerImpl<TStream>(result);
    serializer.serialize(item);
}
}
