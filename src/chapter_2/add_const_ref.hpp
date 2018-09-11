#include <boost/type_traits.hpp>

namespace detail {

template <bool is_function = true>
struct add_const_ref_helper
{
    using type = bool;
};

template <>
struct add_const_ref_helper<false>
{
    using type = int;
};

} // namespace detail

template <typename T>
struct add_const_ref : 
    public detail::add_const_ref_helper<boost::is_function<T>::value || boost::is_member_function_pointer<T>::value>
{
};

