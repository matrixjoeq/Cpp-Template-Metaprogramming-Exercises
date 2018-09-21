
#include <iostream>

#include <boost/type_traits.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/vector.hpp>

#include "add_const_ref.hpp"
#include "replace_type.hpp"
#include "polymorphic_downcast.hpp"
#include "type_descriptor.hpp"

namespace {

namespace mpl = boost::mpl;

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
    std::cout << "Exercise 2-0\n";
    using TypeMap = mpl::map<
        mpl::pair< T,                   T const&          >,
        mpl::pair< const T,             T const&          >,
        mpl::pair< volatile T,          volatile T const& >,
        mpl::pair< const volatile T,    volatile T const& >>;
    mpl::for_each<TypeMap>(AddConstRef());

    using RefMap = mpl::map<
        mpl::pair< T&,                  T&                >,
        mpl::pair< const T&,            const T&          >,
        mpl::pair< volatile T&,         volatile T&       >,
        mpl::pair< const volatile T&,   const volatile T& >>;
    mpl::for_each<RefMap>(AddConstRef());

    using PointerMap = mpl::map<
        mpl::pair< T*,                  T* const&                >,
        mpl::pair< const T*,            const T* const&          >,
        mpl::pair< volatile T*,         volatile T* const&       >,
        mpl::pair< const volatile T*,   const volatile T* const& >,
        mpl::pair< T* const,            T* const&                >,
        mpl::pair< T* volatile,         T* const volatile&       >,
        mpl::pair< T* const volatile,   T* const volatile&       >>;
    mpl::for_each<PointerMap>(AddConstRef());

    using ArrayMap = mpl::map<
        mpl::pair< T[],                 T const (&)[]          >,
        mpl::pair< const T[],           T const (&)[]          >,
        mpl::pair< volatile T[],        volatile T const (&)[] >,
        mpl::pair< const volatile T[],  volatile T const (&)[] >>;
    mpl::for_each<ArrayMap>(AddConstRef());

    using PointerArrayMap = mpl::map<
        mpl::pair< T*[],                T* const (&)[]                >,
        mpl::pair< const T*[],          const T* const (&)[]          >,
        mpl::pair< volatile T*[],       volatile T* const (&)[]       >,
        mpl::pair< const volatile T*[], const volatile T* const (&)[] >,
        mpl::pair< T* const[],          T* const (&)[]                >,
        mpl::pair< T* volatile[],       T* const volatile (&)[]       >,
        mpl::pair< T* const volatile[], T* const volatile (&)[]       >>;
    mpl::for_each<PointerArrayMap>(AddConstRef());

    using RRefMap = mpl::map<
        mpl::pair< T&&,                 T&&                >,
        mpl::pair< const T&&,           const T&&          >,
        mpl::pair< volatile T&&,        volatile T&&       >,
        mpl::pair< const volatile T&&,  const volatile T&& >>;
    mpl::for_each<RRefMap>(AddConstRef());
}

struct ReplaceType
{
    template <typename T>
    void operator()(T&)
    {
        using OriginType  = typename mpl::at<T, mpl::int_<0>>::type;
        using FindType    = typename mpl::at<T, mpl::int_<1>>::type;
        using ReplaceType = typename mpl::at<T, mpl::int_<2>>::type;
        using ExpectType  = typename mpl::at<T, mpl::int_<3>>::type;
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
    std::cout << "Exercise 2-1\n";
    using PRTL = mpl::vector<
        //           O    F    R    E
        mpl::vector< U,   U,   V,   V   >,
        mpl::vector< U*,  U*,  V,   V   >,
        mpl::vector< U&,  U&,  V,   V   >,
        mpl::vector< U*&, U*&, V,   V   >,
        mpl::vector< U*,  U,   V,   V*  >,
        mpl::vector< U*,  U,   V*,  V** >,
        mpl::vector< U*,  U,   V&,  V*  >,   // add_ptr<V&> = V*
        mpl::vector< U*,  U,   V*&, V** >,   // add_ptr<V*&> = V**
        mpl::vector< U&,  U,   V,   V&  >,
        mpl::vector< U&,  U,   V*,  V*& >,
        mpl::vector< U&,  U,   V&,  V&  >,   // add_ref<V&> = V&
        mpl::vector< U&,  U,   V*&, V*& >,   // add_ref<V*&> = V*&
        mpl::vector< U*&, U,   V,   V*& >,
        mpl::vector< U*&, U,   V*,  V**&>,
        mpl::vector< U*&, U,   V&,  V*& >,   // add_ptr<V&> = V*, add_ref<V*> = V*&
        mpl::vector< U*&, U,   V*&, V**&>,   // add_ptr<V*&> = V**, add_ref<V**> = V**&
        mpl::vector< U*&, U*,  V,   V&  >,
        mpl::vector< U*&, U*,  V*,  V*& >,
        mpl::vector< U*&, U*,  V&,  V&  >,   // add_ptr<V&> = V*, add_ref<V&> = V&
        mpl::vector< U*&, U*,  V*&, V*& >>;
    mpl::for_each<PRTL>(ReplaceType());

    using CVTL = mpl::vector<
        //           origin type       find type         replace type      expect type
        mpl::vector< const U,          U,                V,                const V          >,
        mpl::vector< const U,          U,                const V,          const V          >,
        mpl::vector< const U,          U,                volatile V,       const volatile V >,
        mpl::vector< const U,          U,                const volatile V, const volatile V >,
        mpl::vector< volatile U,       U,                V,                volatile V       >,
        mpl::vector< volatile U,       U,                const V,          const volatile V >,
        mpl::vector< volatile U,       U,                volatile V,       volatile V       >,
        mpl::vector< volatile U,       U,                const volatile V, const volatile V >,
        mpl::vector< const volatile U, U,                V,                const volatile V >,
        mpl::vector< const volatile U, U,                const V,          const volatile V >,
        mpl::vector< const volatile U, U,                volatile V,       const volatile V >,
        mpl::vector< const volatile U, U,                const volatile V, const volatile V >,
        mpl::vector< const U,          const U,          V,                V                >,
        mpl::vector< volatile U,       volatile U,       V,                V                >,
        mpl::vector< const volatile U, const volatile U, V,                V                >,
        mpl::vector< const volatile U, const U,          V,                volatile V       >,
        mpl::vector< const volatile U, volatile U,       V,                const V          >,
        mpl::vector< U,                U,                const V,          const V          >,
        mpl::vector< U,                U,                volatile V,       volatile V       >,
        mpl::vector< U,                U,                const volatile V, const volatile V >>;
    mpl::for_each<CVTL>(ReplaceType());

    using CPTL = mpl::vector<
        mpl::vector< const U*, U,       V,        const V*  >,
        mpl::vector< const U*, U*,      V,        const V   >,  // SHOULD be INVALID
                                                                // it's pointer to const U, NOT const pointer to U
        mpl::vector< const U*, const U, V,        V*        >,
        mpl::vector< const U*, U,       V*,       V* const* >,  // add_const(V*) = V* const
        mpl::vector< const U*, U*,      V*,       V* const  >,  // SHOULD be INVALID
        mpl::vector< const U*, const U, V*,       V**       >,
        mpl::vector< const U*, U,       V&,       V*        >,  // add_const(V&) = V&, add_ptr(V&) = V*
        mpl::vector< const U*, U*,      V&,       V&        >,  // SHOULD be INVALID
        mpl::vector< const U*, const U, V&,       V*        >,  // add_ptr<V&> = V*
        mpl::vector< const U*, U,       V*&,      V**       >,  // add_const<V*&> = V*&, add_ptr<V*&> = V**
        mpl::vector< const U*, U*,      V*&,      V*&       >,
        mpl::vector< const U*, const U, V*&,      V**       >,  // add_ptr<V*&> = V**
        mpl::vector< U* const, U,       V,        V* const  >,
        mpl::vector< U* const, U*,      V,        const V   >,
        mpl::vector< U* const, U,       V*,       V** const >,
        mpl::vector< U* const, U*,      V*,       V* const  >,
        mpl::vector< U* const, U,       V&,       V* const  >,  // add_ptr<V&> = V*
        mpl::vector< U* const, U*,      V&,       V&        >,  // add_const<V&> = V&
        mpl::vector< U* const, U,       V*&,      V** const >,  // add_ptr<V*&> = V**
        mpl::vector< U* const, U*,      V*&,      V*&       >>; // add_const<V*&> = V*&
    mpl::for_each<CPTL>(ReplaceType());

    using VPTL = mpl::vector<
        mpl::vector< volatile U*, U,          V,        volatile V*  >,
        mpl::vector< volatile U*, U*,         V,        volatile V   >,  // SHOULD be INVALID, it's pointer to volatile U
                                                                         // NOT volatile pointer to U
        mpl::vector< volatile U*, volatile U, V,        V*           >,
        mpl::vector< volatile U*, U,          V*,       V* volatile* >,  // add_volatile(V*) = V* volatile
        mpl::vector< volatile U*, U*,         V*,       V* volatile  >,  // SHOULD be INVALID
        mpl::vector< volatile U*, volatile U, V*,       V**          >,
        mpl::vector< volatile U*, U,          V&,       V*           >,  // add_volatile(V&) = V&, add_ptr(V&) = V*
        mpl::vector< volatile U*, U*,         V&,       V&           >,  // SHOULD be INVALID
        mpl::vector< volatile U*, volatile U, V&,       V*           >,  // add_ptr<V&> = V*
        mpl::vector< volatile U*, U,          V*&,      V**          >,  // add_volatile<V*&> = V*&, add_ptr<V*&> = V**
        mpl::vector< volatile U*, U*,         V*&,      V*&          >,
        mpl::vector< volatile U*, volatile U, V*&,      V**          >,  // add_ptr<V*&> = V**
        mpl::vector< U* volatile, U,          V,        V* volatile  >,
        mpl::vector< U* volatile, U*,         V,        volatile V   >,
        mpl::vector< U* volatile, U,          V*,       V** volatile >,
        mpl::vector< U* volatile, U*,         V*,       V* volatile  >,
        mpl::vector< U* volatile, U,          V&,       V* volatile  >,  // add_ptr<V&> = V*
        mpl::vector< U* volatile, U*,         V&,       V&           >,  // add_volatile<V&> = V&
        mpl::vector< U* volatile, U,          V*&,      V** volatile >,  // add_ptr<V*&> = V**
        mpl::vector< U* volatile, U*,         V*&,      V*&          >>; // add_volatile<V*&> = V*&
    mpl::for_each<VPTL>(ReplaceType());

    using CRTL = mpl::vector<
        mpl::vector< const U&, U,       V,        const V&  >,
        mpl::vector< const U&, U&,      V,        const V   >,
        mpl::vector< const U&, const U, V,        V&        >,
        mpl::vector< const U&, U,       V*,       V* const& >,  // add_const<V*> = V* const
        mpl::vector< const U&, U&,      V*,       V* const  >,
        mpl::vector< const U&, const U, V*,       V*&       >,
        mpl::vector< const U&, U,       V&,       V&        >,  // add_const<V&> = V&, add_ref<V&> = V&
        mpl::vector< const U&, U&,      V&,       V&        >,  // add_const<V&> = V&
        mpl::vector< const U&, const U, V&,       V&        >,  // add_ref<V&> = V&
        mpl::vector< const U&, U,       V*&,      V*&       >,
        mpl::vector< const U&, U&,      V*&,      V*&       >,  // add_const<V*&> = V*&
        mpl::vector< const U&, const U, V*&,      V*&       >>; // add_ref<V*&> = V*&
    mpl::for_each<CRTL>(ReplaceType());

    using VRTL = mpl::vector<
        mpl::vector< volatile U&, U,          V,        volatile V&  >,
        mpl::vector< volatile U&, U&,         V,        volatile V   >,
        mpl::vector< volatile U&, volatile U, V,        V&           >,
        mpl::vector< volatile U&, U,          V*,       V* volatile& >,  // add_volatile<V*> = V* volatile
        mpl::vector< volatile U&, U&,         V*,       V* volatile  >,
        mpl::vector< volatile U&, volatile U, V*,       V*&          >,
        mpl::vector< volatile U&, U,          V&,       V&           >,  // add_volatile<V&> = V&, add_ref<V&> = V&
        mpl::vector< volatile U&, U&,         V&,       V&           >,  // add_volatile<V&> = V&
        mpl::vector< volatile U&, volatile U, V&,       V&           >,  // add_ref<V&> = V&
        mpl::vector< volatile U&, U,          V*&,      V*&          >,
        mpl::vector< volatile U&, U&,         V*&,      V*&          >,  // add_volatile<V*&> = V*&
        mpl::vector< volatile U&, volatile U, V*&,      V*&          >>; // add_ref<V*&> = V*&
    mpl::for_each<VRTL>(ReplaceType());

    static constexpr size_t N = 10;
    static constexpr size_t M = 5;
    using ArrayTL = mpl::vector<
        mpl::vector< U[N],  U,     V,    V[N]    >,
        mpl::vector< U[N],  U,     V*,   V*[N]   >,
        mpl::vector< U[N],  U,     V[M], V[N][M] >,
        mpl::vector< U[N],  U[N],  V,    V       >,
        mpl::vector< U*[N], U,     V,    V*[N]   >,
        mpl::vector< U*[N], U,     V*,   V**[N]  >,
        mpl::vector< U*[N], U*,    V,    V[N]    >,
        mpl::vector< U*[N], U*,    V*,   V*[N]   >,
        mpl::vector< U*[N], U*,    V[M], V[N][M] >,
        mpl::vector< U*[N], U*[N], V,    V       >>;
    mpl::for_each<ArrayTL>(ReplaceType());

    using CVArrayTL = mpl::vector<
        mpl::vector< const U[N],  U, V, const V[N]  >,
        mpl::vector< const U*[N], U, V, const V*[N] >,
        mpl::vector< U* const[N], U, V, V* const[N] >>;
    mpl::for_each<CVArrayTL>(ReplaceType());

    using FuncTL = mpl::vector<
        mpl::vector< U (*)(),     U, V, V (*)()     >,
        mpl::vector< U (*)(U),    U, V, V (*)(V)    >,
        mpl::vector< U (*)(U, U), U, V, V (*)(V, V) >>;
    mpl::for_each<FuncTL>(ReplaceType());
}

struct Base { virtual ~Base() {} };
struct DerivedOne : Base { int i; };
struct DerivedTwo : Base { int i[2]; };

void test_polymorphic_downcast()
{
    std::cout << "Exercise 2-2\n";
    DerivedOne d1;
    DerivedTwo d2;

    Base* pb1 = &d1;
    Base* pb2 = &d2;
    Base& rb1 = d1;
    Base& rb2 = d2;

    DerivedOne* pd1 = polymorphic_downcast<DerivedOne*>(pb1);
    (void)pd1;
    DerivedTwo* pd2 = polymorphic_downcast<DerivedTwo*>(pb2);
    (void)pd2;

    DerivedOne& rd1 = polymorphic_downcast<DerivedOne&>(rb1);
    (void)rd1;
    DerivedTwo& rd2 = polymorphic_downcast<DerivedTwo&>(rb2);
    (void)rd2;
}

struct TypeDescriptor
{
    template <typename T>
    void operator()(T&)
    {
        std::cout << "T: "                                      << type_descriptor<T>()                                     << std::endl;
        std::cout << "T[]: "                                    << type_descriptor<T[]>()                                   << std::endl;

        std::cout << "T(&)[]: "                                 << type_descriptor<T(&)[]>()                                << std::endl;
        std::cout << "T*: "                                     << type_descriptor<T*>()                                    << std::endl;
        std::cout << "T*[]: "                                   << type_descriptor<T*[]>()                                  << std::endl;
        std::cout << "T*(&)[]: "                                << type_descriptor<T*(&)[]>()                               << std::endl;
        std::cout << "T&: "                                     << type_descriptor<T&>()                                    << std::endl;
        std::cout << "T*&: "                                    << type_descriptor<T*&>()                                   << std::endl;
        std::cout << "const T: "                                << type_descriptor<const T>()                               << std::endl;
        std::cout << "volatile T: "                             << type_descriptor<volatile T>()                            << std::endl;
        std::cout << "const volatile T: "                       << type_descriptor<const volatile T>()                      << std::endl;
        std::cout << "const T[]: "                              << type_descriptor<const T[]>()                             << std::endl;
        std::cout << "volatile T[]: "                           << type_descriptor<volatile T[]>()                          << std::endl;
        std::cout << "const volatile T[]: "                     << type_descriptor<const volatile T[]>()                    << std::endl;
        std::cout << "T* const: "                               << type_descriptor<T* const>()                              << std::endl;
        std::cout << "T* volatile: "                            << type_descriptor<T* volatile>()                           << std::endl;
        std::cout << "T* const volatile: "                      << type_descriptor<T* const volatile>()                     << std::endl;
        std::cout << "T* const[]: "                             << type_descriptor<T* const[]>()                            << std::endl;
        std::cout << "T* volatile[]: "                          << type_descriptor<T* volatile[]>()                         << std::endl;
        std::cout << "T* const volatile[]: "                    << type_descriptor<T* const volatile[]>()                   << std::endl;
        std::cout << "const T*: "                               << type_descriptor<const T*>()                              << std::endl;
        std::cout << "volatile T*: "                            << type_descriptor<volatile T*>()                           << std::endl;
        std::cout << "const volatile T*: "                      << type_descriptor<const volatile T*>()                     << std::endl;
        std::cout << "const T*[]: "                             << type_descriptor<const T*[]>()                            << std::endl;
        std::cout << "volatile T*[]: "                          << type_descriptor<volatile T*[]>()                         << std::endl;
        std::cout << "const volatile T*[]: "                    << type_descriptor<const volatile T*[]>()                   << std::endl;
        std::cout << "const T* const: "                         << type_descriptor<const T* const>()                        << std::endl;
        std::cout << "const T* volatile: "                      << type_descriptor<const T* volatile>()                     << std::endl;
        std::cout << "const T* const volatile: "                << type_descriptor<const T* const volatile>()               << std::endl;
        std::cout << "const T* const[]: "                       << type_descriptor<const T* const[]>()                      << std::endl;
        std::cout << "const T* volatile[]: "                    << type_descriptor<const T* volatile[]>()                   << std::endl;
        std::cout << "const T* const volatile[]: "              << type_descriptor<const T* const volatile[]>()             << std::endl;
        std::cout << "volatile T* const: "                      << type_descriptor<volatile T* const>()                     << std::endl;
        std::cout << "volatile T* volatile: "                   << type_descriptor<volatile T* volatile>()                  << std::endl;
        std::cout << "volatile T* const volatile: "             << type_descriptor<volatile T* const volatile>()            << std::endl;
        std::cout << "volatile T* const[]: "                    << type_descriptor<volatile T* const[]>()                   << std::endl;
        std::cout << "volatile T* volatile[]: "                 << type_descriptor<volatile T* volatile[]>()                << std::endl;
        std::cout << "volatile T* const volatile[]: "           << type_descriptor<volatile T* const volatile[]>()          << std::endl;
        std::cout << "const volatile T* const: "                << type_descriptor<const volatile T* const>()               << std::endl;
        std::cout << "const volatile T* volatile: "             << type_descriptor<const volatile T* volatile>()            << std::endl;
        std::cout << "const volatile T* const volatile: "       << type_descriptor<const volatile T* const volatile>()      << std::endl;
        std::cout << "const volatile T* const[]: "              << type_descriptor<const volatile T* const[]>()             << std::endl;
        std::cout << "const volatile T* volatile[]: "           << type_descriptor<const volatile T* volatile[]>()          << std::endl;
        std::cout << "const volatile T* const volatile[]: "     << type_descriptor<const volatile T* const volatile[]>()    << std::endl;
        std::cout << "const T&: "                               << type_descriptor<const T&>()                              << std::endl;
        std::cout << "volatile T&: "                            << type_descriptor<volatile T&>()                           << std::endl;
        std::cout << "const volatile T&: "                      << type_descriptor<const volatile T&>()                     << std::endl;
        std::cout << "const volatile T* const volatile&: "      << type_descriptor<const volatile T* const volatile&>()     << std::endl;
        std::cout << "const T(&)[]: "                           << type_descriptor<const T(&)[]>()                          << std::endl;
        std::cout << "volatile T(&)[]: "                        << type_descriptor<volatile T(&)[]>()                       << std::endl;
        std::cout << "const volatile T(&)[]: "                  << type_descriptor<const volatile T(&)[]>()                 << std::endl;
        std::cout << "const volatile T* const volatile(&)[]: "  << type_descriptor<const volatile T* const volatile(&)[]>() << std::endl;

        std::cout << "T (): " << type_descriptor<T ()>() << std::endl;
    }
};

void test_type_descriptor()
{
    std::cout << "Exercise 2-3\n";
    using TL = mpl::vector<char>; // short, int, long
    mpl::for_each<TL>(TypeDescriptor());
}

} // namespace

int main()
{
    //test_add_const_ref<int>();
    //test_replace_type<int, float>();
    //test_polymorphic_downcast();
    test_type_descriptor();

    return 0;
}
