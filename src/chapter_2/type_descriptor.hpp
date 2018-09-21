
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
#include <sstream>

namespace detail {

template <typename T>
struct type_descriptor_impl;

template <>
struct type_descriptor_impl<char>
{
    static constexpr const char* const value = "char";
};

template <>
struct type_descriptor_impl<short>
{
    static constexpr const char* const value = "short";
};

template <>
struct type_descriptor_impl<int>
{
    static constexpr const char* const value = "int";
};

template <>
struct type_descriptor_impl<long>
{
    static constexpr const char* const value = "long";
};

template <typename T>
struct remove_all_pointer
{
    using type = T;
};

template <typename T>
struct remove_all_pointer<T*>
{
    using type = typename remove_all_pointer<T>::type;
};

template <typename T>
struct remove_all_pointer<T* const>
{
    using type = typename remove_all_pointer<T>::type;
};

template <typename T>
struct remove_all_pointer<T* volatile>
{
    using type = typename remove_all_pointer<T>::type;
};

template <typename T>
struct remove_all_pointer<T* const volatile>
{
    using type = typename remove_all_pointer<T>::type;
};

template <typename T>
using remove_all_pointer_t = typename remove_all_pointer<T>::type;

} // namespace detail

template <typename T>
struct type_descriptor
{
private:
    using type = boost::remove_cv_t<
        boost::remove_pointer_t<
            boost::remove_all_extents_t<
                boost::remove_cv_ref_t<T>
            >
        >
    >;

public:
    static constexpr const char* const value = detail::type_descriptor_impl<type>::value;
};

template <typename R, typename... Args>
struct type_descriptor<R(Args...)>
{
    static constexpr const char* const value = "";
};

template <typename R, typename... Args>
struct type_descriptor<R(*)(Args...)>
{
    static constexpr const char* const value = "";
};

template <typename T, typename boost::enable_if_c<
    !boost::is_same<T, char>::value  &&
    !boost::is_same<T, int>::value   &&
    !boost::is_same<T, short>::value &&
    !boost::is_same<T, long>::value  &&
    !boost::is_function<T>::value>::type* = nullptr>
inline std::ostream& operator<<(std::ostream& os, type_descriptor<T> const&)
{
    std::ostringstream oss;

    // Extract reference
    using OriginType = T;
    if (boost::is_reference<OriginType>::value) {
        if (boost::is_lvalue_reference<OriginType>::value) {
            oss << "lvalue ";
        }
        else if (boost::is_rvalue_reference<OriginType>::value) {
            oss << "rvalue ";
        }
        oss << "reference to ";
    }

    // Extract extents
    using NoRefType = boost::remove_reference_t<OriginType>;
    if (boost::is_array<NoRefType>::value) {
        oss << "array of ";
    }

    // Extract pointer
    using NoBoundType = boost::remove_all_extents_t<NoRefType>;
    if (boost::is_pointer<NoBoundType>::value) {

        if (boost::is_const<NoBoundType>::value) {
            oss << "const ";
        }

        if (boost::is_volatile<NoBoundType>::value) {
            oss << "volatile ";
        }

        oss << "pointer to ";
    }

    // Extract value
    using CVType = detail::remove_all_pointer_t<NoBoundType>;
    if (boost::is_const<CVType>::value) {
        oss << "const ";
    }

    if (boost::is_volatile<CVType>::value) {
        oss << "volatile ";
    }

    using ValueType = boost::remove_cv_t<CVType>;
    oss << type_descriptor<ValueType>();

    os << oss.str();

    return os;
}

template <typename T, typename boost::enable_if<boost::is_function<T>>::type* = nullptr>
inline std::ostream& operator<<(std::ostream& os, type_descriptor<T> const &)
{
    std::ostringstream oss;
    oss << "function returning " << type_descriptor<typename boost::function_traits<T>::result_type>();
    os << oss.str();
    return os;
}

template <typename T, typename boost::enable_if_c<
    boost::is_same<T, char>::value  ||
    boost::is_same<T, int>::value   ||
    boost::is_same<T, short>::value ||
    boost::is_same<T, long>::value>::type* = nullptr>
inline std::ostream& operator<<(std::ostream& os, type_descriptor<T> const &)
{
    return os << type_descriptor<T>::value;
}

#endif  // __CPP_META_TYPE_DESCRIPTOR_HPP__
