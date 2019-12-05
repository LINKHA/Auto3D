#pragma once
#include "Core/RTTI/StdTypeTraits.h"

#include <type_traits>
namespace Auto3D
{

template<typename T>
struct FIsFunctionPtr : std::integral_constant<bool, std::is_pointer<T>::value &&
	std::is_function<RemovePointer<T>>::value>
{
};

}