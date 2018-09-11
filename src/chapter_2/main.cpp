
#include <iostream>
#include <boost/type_traits.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/for_each.hpp>
#include "add_const_ref.hpp"

struct AddConstRef
{
    template <typename T>
    void operator()(T&)
    {
        using ConstRef = typename add_const_ref<typename T::first>::type;
        //std::cout << typeid(ConstRef).name() << std::endl;
        BOOST_STATIC_ASSERT((boost::is_same<ConstRef, typename T::second>::value));
    }
};

template <typename T>
void test_add_const_ref()
{
    using TypeMap = boost::mpl::map<
        boost::mpl::pair<T,                   T const&                     >,
        boost::mpl::pair<const T,             T const&                     >,
        boost::mpl::pair<volatile T,          volatile T const&            >,
        boost::mpl::pair<const volatile T,    volatile T const&            >>;
    boost::mpl::for_each<TypeMap>(AddConstRef());

    using RefMap = boost::mpl::map<
        boost::mpl::pair<T&,                  T const&                     >,
        boost::mpl::pair<const T&,            T const&                     >,
        boost::mpl::pair<volatile T&,         volatile T const&            >,
        boost::mpl::pair<const volatile T&,   volatile T const&            >>;
    boost::mpl::for_each<RefMap>(AddConstRef());

    using PointerMap = boost::mpl::map<
        boost::mpl::pair<T*,                  T* const&                    >,
        boost::mpl::pair<const T*,            const T* const&              >,
        boost::mpl::pair<volatile T*,         volatile T* const&           >,
        boost::mpl::pair<const volatile T*,   const volatile T* const&     >,
        boost::mpl::pair<T* const,            T* const&                    >,
        boost::mpl::pair<T* volatile,         T* const volatile&           >,
        boost::mpl::pair<T* const volatile,   T* const volatile&           >>;
    boost::mpl::for_each<PointerMap>(AddConstRef());

    using ArrayMap = boost::mpl::map<
        boost::mpl::pair<T[],                 T const (&)[]                >,
        boost::mpl::pair<const T[],           T const (&)[]                >,
        boost::mpl::pair<volatile T[],        volatile T const (&)[]       >,
        boost::mpl::pair<const volatile T[],  volatile T const (&)[]       >>;
    boost::mpl::for_each<ArrayMap>(AddConstRef());

    using PointerArrayMap = boost::mpl::map<
        boost::mpl::pair<T*[],                T* const (&)[]               >,
        boost::mpl::pair<const T*[],          const T* const (&)[]         >,
        boost::mpl::pair<volatile T*[],       volatile T* const (&)[]      >,
        boost::mpl::pair<const volatile T*[], const volatile T* const (&)[]>,
        boost::mpl::pair<T* const[],          T* const (&)[]               >,
        boost::mpl::pair<T* volatile[],       T* const volatile (&)[]      >,
        boost::mpl::pair<T* const volatile[], T* const volatile (&)[]      >>;
    boost::mpl::for_each<PointerArrayMap>(AddConstRef());

    using RRefMap = boost::mpl::map<
        boost::mpl::pair<T&&,                 T const&&                    >,
        boost::mpl::pair<const T&&,           T const&&>,
        boost::mpl::pair<volatile T&&,        volatile T const&&>,
        boost::mpl::pair<const volatile T&&,  volatile T const&&>>;
    boost::mpl::for_each<RRefMap>(AddConstRef());
}

int main()
{
    test_add_const_ref<int>();

    return 0;
}
