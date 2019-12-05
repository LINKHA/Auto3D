#include <type_traits>

#include "Core/RTTI/Type.h"


namespace Auto3D
{

FType::FType()noexcept :
	_typeData(data)
{
}

FType::FType(FTypeData* data)noexcept:
	_typeData(data)
{

}

template<typename _Ty> FType FType::Get() noexcept
{
	using NonRefType = typename std::remove_cv<typename std::remove_reference<_Ty>::type>::type;

	return CreateOrGetType<NonRefType>();
}

inline FType CreateType(FTypeData* data) noexcept
{
	return data ? FType(data) : FType();
}

}