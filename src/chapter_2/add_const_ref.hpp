
#ifndef __CPP_META_ADD_CONST_REF_HPP__
#define __CPP_META_ADD_CONST_REF_HPP__

/**
 * Write a unary meta-function add_const_ref<T> that returns T if it is a
 * reference type, and otherwise returns T const&.  Write a program to test
 * your meta-function.  Hint: you can use boost::is_same to test the results.
 */

namespace detail {

template <typename T, bool is_ref = false>
struct add_const_ref_impl
{
    using type = T const&;
};

template <typename T>
struct add_const_ref_impl<T, true>
{
    using type = T;
};

template <typename T>
struct add_const_ref : add_const_ref_impl<T> {};

template <typename T>
struct add_const_ref<T const> : add_const_ref_impl<T> {};

template <typename T>
struct add_const_ref<T&> : add_const_ref_impl<T&, true> {};

template <typename T>
struct add_const_ref<T&&> : add_const_ref_impl<T&&, true> {};

} // namespace detail

template <typename T>
struct add_const_ref : detail::add_const_ref<T> {};

#endif  // __CPP_META_ADD_CONST_REF_HPP__
