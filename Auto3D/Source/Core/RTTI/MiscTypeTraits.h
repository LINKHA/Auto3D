#pragma once
#include "Core/RTTI/StdTypeTraits.h"
#include "Core/RTTI/FunctionTraits.h"

#include <type_traits>

namespace Auto3D
{
namespace RTTI
{

template<typename T, typename Enable = void>
struct FRawType
{
	using Type = RemoveCV<T>;
};

template<typename T> struct FRawType<T, EnableIf<std::is_pointer<T>::value && !FIsFunctionPtr<T>::value>>
{
	using type = typename FRawType<RemovePointer<T>>::type;
};

template<typename T> struct FRawType<T, EnableIf<std::is_reference<T>::value> >
{
	using type = typename FRawType<RemoveReference<T> >::type;
};


template<typename T>
using RawType = typename FRawType<T>::type;

template<typename T, typename Enable = void>
struct FRawArrayType { using Type = FRawType<T>; };

template<typename T>
using RawArrayType = typename FRawArrayType<T>::Type;

struct PointerCountImpl
{
	static constexpr std::size_t size = 0;
};

/////////////////////////////////////////////////////////////////////////////////////
// pointer_count<T>::value Returns the number of pointers for a type
// e.g. pointer_count<char**>::value => 2
//      pointer_count<char*>::value  => 1
//      pointer_count<char>::value   => 0
template<typename T, typename Enable = void>
struct FPointerCountImpl
{
	static constexpr std::size_t size = 0;
};

template<typename T>
struct FPointerCountImpl<T, EnableIf<std::is_pointer<T>::value &&
	!FIsFunctionPtr<T>::value &&
	!std::is_member_pointer<T>::value>>
{
	static constexpr std::size_t size = FPointerCountImpl<RemovePointer<T> >::size + 1;
};

template<typename T>
using PointerCount = std::integral_constant<size_t, FPointerCountImpl<T>::size>;

}
}