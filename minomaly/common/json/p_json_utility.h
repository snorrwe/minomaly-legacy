#pragma once
#include <iostream>
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
    auto found = false;
    for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i) {
        constexpr auto property = std::get<i>(T::jsonProperties());
        if (strEqual(property.name, name))
        {
            found = true;
            f(property);
        }
    });
    if (!found)
    {
        throw UnexpectedPropertyName(name);
    }
}

template <typename T>
class IsJsonParseble
{
    using Yes = char;
    struct No
    {
        char _[2];
    };

    template <typename C>
    static Yes test(decltype(&C::jsonProperties));
    static No test(...);

public:
    enum
    {
        value = sizeof(test<T>(nullptr)) == sizeof(Yes)
    };
};
}
