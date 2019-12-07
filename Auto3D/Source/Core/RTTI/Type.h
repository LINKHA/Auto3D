#pragma once
#include "AutoConfig.h"
#include "StdTypeTraits.h"
#include "Core/RTTI/TypeData.h"

namespace Auto3D
{
namespace RTTI
{

class AUTO_API FType
{
public:

	/// Constructs an empty and invalid type object.
	FType() noexcept;
	/// Constructs a valid type object.
	explicit FType(FTypeData* data)noexcept;

	template<typename _Ty> static FType Get() noexcept;

private:
	FTypeData* _typeData;
};

inline FType CreateType(FTypeData* data) noexcept;

template<typename _Ty>
using IsCcompleteType = std::integral_constant<bool, !std::is_function<_Ty>::value && !std::is_same<_Ty, void>::value>;

template<typename _Ty>inline EnableIf<IsCcompleteType<_Ty>::value, FType>
	CreateOrGetType() noexcept
	{
		// when you get an error here, then the type was not completely defined
		// (a forward declaration is not enough because base_classes will not be found)
		using TypeMustBeComplete = char[sizeof(_Ty) ? 1 : -1];
		(void) sizeof(TypeMustBeComplete);

		static const FType val = create_type(GetRegistrationManager().AddItem(make_type_data<T>()));
		return val;
	}

	inline static FType GetInvalidType() noexcept { return CreateType(nullptr); }
}
}