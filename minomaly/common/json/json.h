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

template <typename T, typename FwIt>
T parse(FwIt begin, FwIt end)
{
    throw "Not implmented";
}
}
