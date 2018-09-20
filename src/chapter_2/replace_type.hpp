
#ifndef __CPP_META_REPLACE_TYPE_HPP__
#define __CPP_META_REPLACE_TYPE_HPP__

#include <boost/type_traits.hpp>

/**
 * Write a ternary meta-function replace_type<c,x,y> that takes an arbitrary
 * compound type c as its first parameter, and replaces all occurrences of a
 * type x within c with y:
 *
 * typedef replace_type<void*, void, int>::type t1; // int*
 * typedef replace_type<
 *   int const*[10]
 *   , int const
 *   , long
 *   >::type t2; // long* [10]
 * typedef replace_type<
 *   char& (*)(char&)
 *   , char&
 *   , long&
 *   >::type t3; // long& (*)(long&)
 */

namespace detail {

template <
    typename T,
    size_t N,
    typename = typename boost::enable_if_c<
        !boost::is_abstract<T>::value &&
        !boost::is_reference<T>::value &&
        !boost::is_function<T>::value &&
        !boost::is_void<T>::value &&
        (N > 0)
    >::type
>
struct add_extent
{
    using type = T[N];
};

template <typename C, typename X, typename Y>
struct replace_type_impl;

template <typename C, typename Y>
struct replace_type_impl<C, C, Y>
{
    using type = Y;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<C[N], C, Y>
{
    using type = typename add_extent<Y, N>::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<const C[N], C, Y>
{
    using type = typename add_extent<typename boost::add_const<Y>::type, N>::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<volatile C[N], C, Y>
{
    using type = typename add_extent<typename boost::add_volatile<Y>::type, N>::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<const volatile C[N], C, Y>
{
    using type = typename add_extent<typename boost::add_cv<Y>::type, N>::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<const volatile C[N], const C, Y>
{
    using type = typename add_extent<typename boost::add_volatile<Y>::type, N>::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<const volatile C[N], volatile C, Y>
{
    using type = typename add_extent<typename boost::add_const<Y>::type, N>::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<C*[N], C, Y>
{
    using type = typename add_extent<typename boost::add_pointer<Y>::type, N>::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<const C*[N], C, Y>
{
    using type = typename add_extent<
        typename boost::add_pointer<
            typename boost::add_const<Y>::type
        >::type,
        N
    >::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<volatile C*[N], C, Y>
{
    using type = typename add_extent<
        typename boost::add_pointer<
            typename boost::add_volatile<Y>::type
        >::type,
        N
    >::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<const volatile C*[N], C, Y>
{
    using type = typename add_extent<
        typename boost::add_pointer<
            typename boost::add_cv<Y>::type
        >::type,
        N
    >::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<const volatile C*[N], const C, Y>
{
    using type = typename add_extent<
        typename boost::add_pointer<
            typename boost::add_volatile<Y>::type
        >::type,
        N
    >::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<const volatile C*[N], volatile C, Y>
{
    using type = typename add_extent<
        typename boost::add_pointer<
            typename boost::add_const<Y>::type
        >::type,
        N
    >::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<C* const[N], C, Y>
{
    using type = typename add_extent<
        typename boost::add_const<
            typename boost::add_pointer<Y>::type
        >::type,
        N
    >::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<C* volatile[N], C, Y>
{
    using type = typename add_extent<
        typename boost::add_volatile<
            typename boost::add_pointer<Y>::type
        >::type,
        N
    >::type;
};

template <typename C, typename Y, size_t N>
struct replace_type_impl<C* const volatile[N], C, Y>
{
    using type = typename add_extent<
        typename boost::add_cv<
            typename boost::add_pointer<Y>::type
        >::type,
        N
    >::type;
};

template <typename C, typename Y>
struct replace_type_impl<const C, C, Y>
{
    using type = typename boost::add_const<Y>::type;
};

template <typename C, typename Y>
struct replace_type_impl<volatile C, C, Y>
{
    using type = typename boost::add_volatile<Y>::type;
};

template <typename C, typename Y>
struct replace_type_impl<const volatile C, C, Y>
{
    using type = typename boost::add_cv<Y>::type;
};

template <typename C, typename Y>
struct replace_type_impl<const volatile C, const C, Y>
{
    using type = typename boost::add_volatile<Y>::type;
};

template <typename C, typename Y>
struct replace_type_impl<const volatile C, volatile C, Y>
{
    using type = typename boost::add_const<Y>::type;
};

template <typename C, typename X, typename Y>
struct replace_type
{
    using type = typename replace_type_impl<C, X, Y>::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C*, X, Y>
{
    using type = typename boost::add_pointer<
        typename replace_type_impl<C, X, Y>::type
    >::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C* const, X, Y>
{
    using type = typename boost::add_const<
        typename boost::add_pointer<
            typename replace_type_impl<C, X, Y>::type
        >::type
    >::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C* volatile, X, Y>
{
    using type = typename boost::add_volatile<
        typename boost::add_pointer<
            typename replace_type_impl<C, X, Y>::type
        >::type
    >::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C* const volatile, X, Y>
{
    using type = typename boost::add_cv<
        typename boost::add_pointer<
            typename replace_type_impl<C, X, Y>::type
        >::type
    >::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C*, X*, Y>
{
    using type = typename replace_type_impl<C, X, Y>::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C* const, X*, Y>
{
    using type = typename boost::add_const<
        typename replace_type_impl<C, X, Y>::type
    >::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C* volatile, X*, Y>
{
    using type = typename boost::add_volatile<
        typename replace_type_impl<C, X, Y>::type
    >::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C* const volatile, X*, Y>
{
    using type = typename boost::add_cv<
        typename replace_type_impl<C, X, Y>::type
    >::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C&, X, Y>
{
    using type = typename boost::add_reference<
        typename replace_type_impl<C, X, Y>::type
    >::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C&, X&, Y>
{
    using type = typename replace_type_impl<C, X, Y>::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C*&, X, Y>
{
    using type = typename boost::add_reference<
        typename boost::add_pointer<
            typename replace_type_impl<C, X, Y>::type
        >::type
    >::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C*&, X*, Y>
{
    using type = typename boost::add_reference<
        typename replace_type_impl<C, X, Y>::type
    >::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C*&, X*&, Y>
{
    using type = typename replace_type_impl<C, X, Y>::type;
};

template <typename R, typename X, typename Y>
struct replace_type<R(*)(), X, Y>
{
    using type = typename replace_type<R, X, Y>::type (*)();
};

template <typename R, typename A0, typename X, typename Y>
struct replace_type<R(*)(A0), X, Y>
{
    using type = typename replace_type<R, X, Y>::type (*)(typename replace_type<A0, X, Y>::type);
};

template <typename R, typename A0, typename A1, typename X, typename Y>
struct replace_type<R(*)(A0, A1), X, Y>
{
    using type = typename replace_type<R, X, Y>::type (*)(
        typename replace_type<A0, X, Y>::type,
        typename replace_type<A1, X, Y>::type);
};

template <typename R, typename A0, typename A1, typename A2, typename X, typename Y>
struct replace_type<R(*)(A0, A1, A2), X, Y>
{
    using type = typename replace_type<R, X, Y>::type (*)(
        typename replace_type<A0, X, Y>::type,
        typename replace_type<A1, X, Y>::type,
        typename replace_type<A2, X, Y>::type);
};

template <typename R, typename A0, typename A1, typename A2, typename A3, typename X, typename Y>
struct replace_type<R(*)(A0, A1, A2, A3), X, Y>
{
    using type = typename replace_type<R, X, Y>::type (*)(
        typename replace_type<A0, X, Y>::type,
        typename replace_type<A1, X, Y>::type,
        typename replace_type<A2, X, Y>::type,
        typename replace_type<A3, X, Y>::type);
};

template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename X, typename Y>
struct replace_type<R(*)(A0, A1, A2, A3, A4), X, Y>
{
    using type = typename replace_type<R, X, Y>::type (*)(
        typename replace_type<A0, X, Y>::type,
        typename replace_type<A1, X, Y>::type,
        typename replace_type<A2, X, Y>::type,
        typename replace_type<A3, X, Y>::type,
        typename replace_type<A4, X, Y>::type);
};

template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4,
          typename A5, typename X, typename Y>
struct replace_type<R(*)(A0, A1, A2, A3, A4, A5), X, Y>
{
    using type = typename replace_type<R, X, Y>::type (*)(
        typename replace_type<A0, X, Y>::type,
        typename replace_type<A1, X, Y>::type,
        typename replace_type<A2, X, Y>::type,
        typename replace_type<A3, X, Y>::type,
        typename replace_type<A4, X, Y>::type,
        typename replace_type<A5, X, Y>::type);
};

template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4,
          typename A5, typename A6, typename X, typename Y>
struct replace_type<R(*)(A0, A1, A2, A3, A4, A5, A6), X, Y>
{
    using type = typename replace_type<R, X, Y>::type (*)(
        typename replace_type<A0, X, Y>::type,
        typename replace_type<A1, X, Y>::type,
        typename replace_type<A2, X, Y>::type,
        typename replace_type<A3, X, Y>::type,
        typename replace_type<A4, X, Y>::type,
        typename replace_type<A5, X, Y>::type,
        typename replace_type<A6, X, Y>::type);
};

template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4,
          typename A5, typename A6, typename A7, typename X, typename Y>
struct replace_type<R(*)(A0, A1, A2, A3, A4, A5, A6, A7), X, Y>
{
    using type = typename replace_type<R, X, Y>::type (*)(
        typename replace_type<A0, X, Y>::type,
        typename replace_type<A1, X, Y>::type,
        typename replace_type<A2, X, Y>::type,
        typename replace_type<A3, X, Y>::type,
        typename replace_type<A4, X, Y>::type,
        typename replace_type<A5, X, Y>::type,
        typename replace_type<A6, X, Y>::type,
        typename replace_type<A7, X, Y>::type);
};

template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4,
          typename A5, typename A6, typename A7, typename A8, typename X, typename Y>
struct replace_type<R(*)(A0, A1, A2, A3, A4, A5, A6, A7, A8), X, Y>
{
    using type = typename replace_type<R, X, Y>::type (*)(
        typename replace_type<A0, X, Y>::type,
        typename replace_type<A1, X, Y>::type,
        typename replace_type<A2, X, Y>::type,
        typename replace_type<A3, X, Y>::type,
        typename replace_type<A4, X, Y>::type,
        typename replace_type<A5, X, Y>::type,
        typename replace_type<A6, X, Y>::type,
        typename replace_type<A7, X, Y>::type,
        typename replace_type<A8, X, Y>::type);
};

template <typename R, typename A0, typename A1, typename A2, typename A3, typename A4,
          typename A5, typename A6, typename A7, typename A8, typename A9, typename X, typename Y>
struct replace_type<R(*)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9), X, Y>
{
    using type = typename replace_type<R, X, Y>::type (*)(
        typename replace_type<A0, X, Y>::type,
        typename replace_type<A1, X, Y>::type,
        typename replace_type<A2, X, Y>::type,
        typename replace_type<A3, X, Y>::type,
        typename replace_type<A4, X, Y>::type,
        typename replace_type<A5, X, Y>::type,
        typename replace_type<A6, X, Y>::type,
        typename replace_type<A7, X, Y>::type,
        typename replace_type<A8, X, Y>::type,
        typename replace_type<A9, X, Y>::type);
};

} // namespace

template <typename C, typename X, typename Y>
struct replace_type : public detail::replace_type<C, X, Y> {};

#endif  // __CPP_META_REPLACE_TYPE_HPP__
