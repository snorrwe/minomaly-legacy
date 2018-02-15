#pragma once
#include <iostream>
#include <map>
#include <string>
#include <tuple>

namespace Mino::Json
{
struct Object;

namespace Private
{
// sequence for
template <typename T, T... S, typename F>
constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f)
{
    using unpack_t = int[];
    (void)unpack_t{(static_cast<void>(f(std::integral_constant<T, S>{})), 0)..., 0};
}

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

// One could overload this function to accept both a getter and a setter instead of a member.
template <typename Class, typename T>
constexpr auto property(T Class::*member, const char* name)
{
    return PropertyImpl<Class, T>{member, name};
}
// unserialize function
template <typename T>
T fromJson(Json::Object const& data)
{
    auto result = T{};

    // We first get the number of properties
    constexpr auto nbProperties = std::tuple_size<decltype(T::jsonProperties())>::value;

    // We iterate on the index sequence of size `nbProperties`
    for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i) {
        // get the property
        constexpr auto property = std::get<i>(T::jsonProperties());

        // get the type of the property
        using Type = typename decltype(property)::Type;

        // set the value to the member
        result.*(property.member) = Json::asAny<Type>(data[property.name]);
    });

    return result;
}

/*
template <typename T>
Json::Object toJson(const T& object)
{
    Json::Object data;

    // We first get the number of properties
    constexpr auto nbProperties = std::tuple_size<decltype(T::jsonProperties())>::value;

    // We iterate on the index sequence of size `nbProperties`
    for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i) {
        // get the property
        constexpr auto property = std::get<i>(T::properties);

        // set the value to the member
        data[property.name] = object.*(property.member);
    });

    return data;
}*/
} // namespace Private
} // namespace Mino
