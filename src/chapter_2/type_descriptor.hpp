
#ifndef __CPP_META_TYPE_DESCRIPTOR_HPP__
#define __CPP_META_TYPE_DESCRIPTOR_HPP__

/**
 * Use the type traits facilities to implement a type_descriptor class
 * template, whose instances, when streamed, print the type of their
 * template parameters:
 *
 * // prints "int"
 * std::cout << type_descriptor<int>();
 *
 * // prints "char*"
 * std::cout << type_descriptor<char*>();
 *
 * // prints "long const*&"
 * std::cout << type_descriptor<long const*&>();
 *
 * You can assume that type_descriptor’s template parameter is limited to
 * compound types built from the following four integral types: char,
 * short int, int, and long int.
 */

#include <iostream>

namespace detail {

template <typename T>
struct type_descriptor_impl;

template <>
struct type_descriptor_impl<char>
{
    static constexpr const char* const value = "char";
};

template <>
struct type_descriptor_impl<short int>
{
    static constexpr const char* const value = "short int";
};

template <>
struct type_descriptor_impl<int>
{
    static constexpr const char* const value = "int";
};

template <>
struct type_descriptor_impl<long int>
{
    static constexpr const char* const value = "long int";
};

template <typename T>
struct type_descriptor : type_descriptor_impl<T> {};

} // namespace detail

template <typename T>
struct type_descriptor : detail::type_descriptor<T>
{
    friend std::ostream& operator<<(std::ostream&, type_descriptor const&);
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, type_descriptor<T> const&)
{
    os << type_descriptor<T>::value;
}

#endif  // __CPP_META_TYPE_DESCRIPTOR_HPP__
