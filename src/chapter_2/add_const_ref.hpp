#include <boost/type_traits.hpp>

template <typename T>
struct add_const_ref
{
    using type = T const&;
};

template <typename T>
struct add_const_ref<T&>
{
    using type = T const&;
};

template <typename T>
struct add_const_ref<T&&>
{
    using type = T const&&;
};
