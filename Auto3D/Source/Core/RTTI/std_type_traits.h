#pragma once
#include "core_prerequisites.h"

#include <type_traits>

namespace rttr
{

namespace detail
{

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// shortcuts to avoid writing typename my_traits<T>::type over and over again
// C++14 has support for this, but we support at the moment C++11

template< bool B, class T, class F >
using conditional_t = typename std::conditional<B, T, F>::type;

template<typename T>
using remove_cv_t = typename std::remove_cv<T>::type;

template<typename T>
using remove_volatile_t = typename std::remove_volatile<T>::type;

template<typename T>
using remove_const_t = typename std::remove_const<T>::type;

template<typename T>
using remove_pointer_t = typename std::remove_pointer<T>::type;

template<typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

template<typename T>
using add_pointer_t = typename std::add_pointer<T>::type;

template< bool B, class T = void >
using enable_if_t = typename std::enable_if<B, T>::type;

template<typename T>
using decay_t = typename std::decay<T>::type;

template<typename T>
using add_const_t = typename std::add_const<T>::type;

template<typename T>
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

/////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr