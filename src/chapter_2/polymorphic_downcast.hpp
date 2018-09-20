
#ifndef __CPP_META_POLYMORPHIC_DOWNCAST_HPP__
#define __CPP_META_POLYMORPHIC_DOWNCAST_HPP__

/**
 * The boost::polymorphic_downcast function template implements a checked
 * version of static_cast intended for downcasting pointers to polymorphic
 * objects:
 * In released software, the assertion disappears and polymorphic_downcast
 * can be as efficient as a simple static_cast.  Use the type traits
 * facilities to write an implementation of the template that allows both
 * pointer and reference arguments.
 */

#include <boost/assert.hpp>
#include <boost/type_traits.hpp>

template <typename Target, typename Source>
inline Target polymorphic_downcast(Source* x)
{
    BOOST_ASSERT((dynamic_cast<Target>(x) == x));
    return static_cast<Target>(x);
}

template <typename Target, typename Source>
inline Target polymorphic_downcast(Source& x)
{
    BOOST_ASSERT((dynamic_cast<typename boost::add_pointer<
        typename boost::remove_reference<Target>::type
    >::type>(&x) == &x));
    return static_cast<Target>(x);
}

#endif  // __CPP_META_POLYMORPHIC_DOWNCAST_HPP__
