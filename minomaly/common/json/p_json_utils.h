#pragma once

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
T parseValue(std::stringstream& stream)
{
    return T{}; // TODO
}

template <>
int parseValue<int>(std::stringstream& stream)
{
    return 0; // TODO
}
}
