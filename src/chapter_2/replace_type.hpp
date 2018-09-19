
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

template <typename C, typename X, typename Y>
struct replace_type_impl
{
protected:
    using replaced_type = C;
};

template <typename C, typename Y>
struct replace_type_impl<C, C, Y>
{
protected:
    using replaced_type = Y;
};

template <typename C, typename Y>
struct replace_type_impl<const C, C, Y>
{
protected:
    using replaced_type = const Y;
};

template <typename C, typename Y>
struct replace_type_impl<volatile C, C, Y>
{
protected:
    using replaced_type = volatile Y;
};

template <typename C, typename Y>
struct replace_type_impl<const volatile C, C, Y>
{
protected:
    using replaced_type = const volatile Y;
};

template <typename C, typename Y>
struct replace_type_impl<const volatile C, const C, Y>
{
protected:
    using replaced_type = volatile Y;
};

template <typename C, typename Y>
struct replace_type_impl<const volatile C, volatile C, Y>
{
protected:
    using replaced_type = const Y;
};

template <typename C, typename X, typename Y>
struct replace_type : replace_type_impl<C, X, Y>
{
    using type = replaced_type;
};

template <typename C, typename X, typename Y>
struct replace_type<C*, X, Y> : replace_type_impl<C, X, Y>
{
    using type = typename boost::add_pointer<replaced_type>::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C&, X, Y> : replace_type_impl<C, X, Y>
{
    using type = typename boost::add_reference<replaced_type>::type;
};

template <typename C, typename X, typename Y>
struct replace_type<C*&, X, Y> : replace_type_impl<C, X, Y>
{
    using type = typename boost::add_reference<typename boost::add_pointer<replaced_type>::type>::type;
};

} // namespace

template <typename C, typename X, typename Y>
struct replace_type : detail::replace_type<C, X, Y> {};

#endif  // __CPP_META_REPLACE_TYPE_HPP__

