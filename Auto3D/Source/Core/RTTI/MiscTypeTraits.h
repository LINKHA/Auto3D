#pragma once
#include "Core/RTTI/StdTypeTraits.h"
#include "Core/RTTI/FunctionTraits.h"

#include <type_traits>

namespace Auto3D
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
using RawType = typename raw_type<T>::type;

template<typename T, typename Enable = void>
struct FRawArrayType { using Type = FRawType<T>; };

template<typename T>
using RawArrayType = typename FRawArrayType<T>::Type;

}