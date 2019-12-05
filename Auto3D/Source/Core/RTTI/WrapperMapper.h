#pragma once
#include <type_traits>

namespace Auto3D
{
struct FInvalidWrapperType { };

template<typename T>
struct FWrapperMapper
{
	using WrappedType = FInvalidWrapperType;
	using Type = T;
};

template<typename _Ty>
using WrapperMapper = typename FWrapperMapper<typename std::remove_cv<typename std::remove_reference<_Ty>::type>::type>::WrappedType;

template<typename _Ty>
using IsWrapper = std::integral_constant<bool, !std::is_same<FInvalidWrapperType, WrapperMapper<_Ty>>::value >;

}