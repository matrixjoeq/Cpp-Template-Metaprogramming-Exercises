
#include <iostream>

#include <boost/type_traits.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/vector.hpp>

#include "add_const_ref.hpp"
#include "replace_type.hpp"

namespace {

struct AddConstRef
{
    template <typename T>
    void operator()(T&)
    {
        using type = typename add_const_ref<typename T::first>::type;
        std::cout << "add_const_ref result: " << typeid(type).name() << std::endl;
        BOOST_STATIC_ASSERT_MSG((boost::is_same<type, typename T::second>::value), 
                                "add_const_ref does not return type as expected");
    }
};

template <typename T>
void test_add_const_ref()
{
    using TypeMap = boost::mpl::map<
        boost::mpl::pair<T,                   T const&                      >,
        boost::mpl::pair<const T,             T const&                      >,
        boost::mpl::pair<volatile T,          volatile T const&             >,
        boost::mpl::pair<const volatile T,    volatile T const&             >>;
    boost::mpl::for_each<TypeMap>(AddConstRef());
    
    using RefMap = boost::mpl::map<
        boost::mpl::pair<T&,                  T&                            >,
        boost::mpl::pair<const T&,            const T&                      >,
        boost::mpl::pair<volatile T&,         volatile T&                   >,
        boost::mpl::pair<const volatile T&,   const volatile T&             >>;
    boost::mpl::for_each<RefMap>(AddConstRef());
    
    using PointerMap = boost::mpl::map<
        boost::mpl::pair<T*,                  T* const&                     >,
        boost::mpl::pair<const T*,            const T* const&               >,
        boost::mpl::pair<volatile T*,         volatile T* const&            >,
        boost::mpl::pair<const volatile T*,   const volatile T* const&      >,
        boost::mpl::pair<T* const,            T* const&                     >,
        boost::mpl::pair<T* volatile,         T* const volatile&            >,
        boost::mpl::pair<T* const volatile,   T* const volatile&            >>;
    boost::mpl::for_each<PointerMap>(AddConstRef());

    using ArrayMap = boost::mpl::map<
        boost::mpl::pair<T[],                 T const (&)[]                 >,
        boost::mpl::pair<const T[],           T const (&)[]                 >,
        boost::mpl::pair<volatile T[],        volatile T const (&)[]        >,
        boost::mpl::pair<const volatile T[],  volatile T const (&)[]        >>;
    boost::mpl::for_each<ArrayMap>(AddConstRef());

    using PointerArrayMap = boost::mpl::map<
        boost::mpl::pair<T*[],                T* const (&)[]                >,
        boost::mpl::pair<const T*[],          const T* const (&)[]          >,
        boost::mpl::pair<volatile T*[],       volatile T* const (&)[]       >,
        boost::mpl::pair<const volatile T*[], const volatile T* const (&)[] >,
        boost::mpl::pair<T* const[],          T* const (&)[]                >,
        boost::mpl::pair<T* volatile[],       T* const volatile (&)[]       >,
        boost::mpl::pair<T* const volatile[], T* const volatile (&)[]       >>;
    boost::mpl::for_each<PointerArrayMap>(AddConstRef());

    using RRefMap = boost::mpl::map<
        boost::mpl::pair<T&&,                 T&&                           >,
        boost::mpl::pair<const T&&,           const T&&                     >,
        boost::mpl::pair<volatile T&&,        volatile T&&                  >,
        boost::mpl::pair<const volatile T&&,  const volatile T&&            >>;
    boost::mpl::for_each<RRefMap>(AddConstRef());
}

struct ReplaceType
{
    template <typename T>
    void operator()(T&)
    {
        using OriginType  = typename boost::mpl::at<T, boost::mpl::int_<0>>::type;
        using FindType    = typename boost::mpl::at<T, boost::mpl::int_<1>>::type;
        using ReplaceType = typename boost::mpl::at<T, boost::mpl::int_<2>>::type;
        using ExpectType  = typename boost::mpl::at<T, boost::mpl::int_<3>>::type;
        using ActualType  = typename replace_type<OriginType, FindType, ReplaceType>::type;
        std::cout << "origin type: " << typeid(OriginType).name();
        std::cout << ", find type: " << typeid(FindType).name();
        std::cout << ", replace type: " << typeid(ReplaceType).name();
        std::cout << ", expect type: " << typeid(ExpectType).name();
        std::cout << ", replace_type result: " << typeid(ActualType).name() << std::endl;
        BOOST_STATIC_ASSERT_MSG((boost::is_same<ActualType, ExpectType>::value), 
                                "replace_type does not return expected type");
    }
};

template <typename U, typename V>
void test_replace_type()
{
    using BasicTypeList = boost::mpl::vector <
        boost::mpl::vector<U, U, V, V  >,
        boost::mpl::vector<U, U, V&, V& >,
        boost::mpl::vector<U, U, V*, V*>,
        boost::mpl::vector<U, U, V*&, V*&>,
        boost::mpl::vector<U*, U, V, V* >,
        boost::mpl::vector<U*, U*, V, V>,
        boost::mpl::vector<U*, U*, V&, V&>,
        boost::mpl::vector<U&,  U, V, V& >,
        boost::mpl::vector<U*&, U, V, V*&>>;
    boost::mpl::for_each<BasicTypeList>(ReplaceType());

    using QualifiedTypeList = boost::mpl::vector <
        //                 origin type       find type         replace type      expect type
        boost::mpl::vector<const U,          U,                V,                const V         >,
        boost::mpl::vector<volatile U,       U,                V,                volatile V      >,
        boost::mpl::vector<const volatile U, U,                V,                const volatile V>,
        boost::mpl::vector<const U,          const U,          V,                V               >,
        boost::mpl::vector<volatile U,       volatile U,       V,                V               >,
        boost::mpl::vector<const volatile U, const volatile U, V,                V               >,
        boost::mpl::vector<const volatile U, const U,          V,                volatile V      >,
        boost::mpl::vector<const volatile U, volatile U,       V,                const V         >,
        boost::mpl::vector<U,                U,                const V,          const V         >,
        boost::mpl::vector<U,                U,                volatile V,       volatile V      >,
        boost::mpl::vector<U,                U,                const volatile V, const volatile V>>;
    boost::mpl::for_each<QualifiedTypeList>(ReplaceType());
}

} // namespace

int main()
{
    //test_add_const_ref<int>();
    test_replace_type<int, float>();

    return 0;
}
