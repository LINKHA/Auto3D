#pragma once
#include <type_traits>

namespace Auto3D
{
namespace RTTI
{

template<typename T>
using RemoveCV = typename std::remove_cv<T>::type;

template<typename T>
using RemovePointer = typename std::remove_pointer<T>::type;

template<typename T>
using RemoveReference = typename std::remove_reference<T>::type;

template< bool B, class T = void >
using EnableIf = typename std::enable_if<B, T>::type;
}
}