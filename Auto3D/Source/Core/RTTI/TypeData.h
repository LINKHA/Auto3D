#pragma once
#include "AutoConfig.h"
#include "Container/Ptr.h"
#include "Container/String.h"
#include "Container/Vector.h"
#include "Core/RTTI/GetCreateVariantFunc.h"
#include "Core/RTTI/WrapperMapper.h"
#include "Core/RTTI/MiscTypeTraits.h"

#include <type_traits>
#include <bitset>

namespace Auto3D
{
namespace RTTI
{

enum class TypeTraitInfos : std::size_t
{
	is_class = 0,
	is_enum,
	is_array,
	is_pointer,
	is_arithmetic,
	is_function_pointer,
	is_member_object_pointer,
	is_member_function_pointer,
	is_associative_container,
	is_sequential_container,
	is_template_instantiation,

	TYPE_TRAIT_COUNT
};

using TypeTraits = std::bitset<static_cast<std::size_t>(TypeTraitInfos::TYPE_TRAIT_COUNT)>;

using CreateVariantFunc = decltype(&FCreateInvalidVariantPolicy::CreateVariant);
using GetBaseTypesFunc = decltype(&base_classes<int>::get_types);
using CreateWrapperFunc = void(*)(const argument& arg, variant& var);
using GetMetadataFunc = TVector<metadata>&(*)(void);
using GetClassDataFunc = class_data & (*)(void);
using VisitTypeFunc = void(*)(type_of_visit, visitor&, const type&);

struct AUTO_API FTypeData : public FRefCounted
{
	FTypeData* _rawTypeData;
	FTypeData* _wrappedType;
	FTypeData* _arrayRawType;

	FString _name;
	FString _typeName;

	size_t _getSizeof;
	size_t _getPointerDimension;

	CreateVariantFunc _createVariant;
	GetBaseTypesFunc _getBaseTypes; // FIXME: this info should not be stored, its just temporarily,
												// thats why we store it as function pointer

	enumeration_wrapper_base*  _enumWrapper;
	GetMetadataFunc    _getMetadata;
	CreateWrapperFunc  _createWrapper;
	VisitTypeFunc      _visitType;

	bool is_valid;
	inline bool type_trait_value(type_trait_infos type_trait) const noexcept { return m_type_traits.test(static_cast<std::size_t>(type_trait)); }


	type_traits m_type_traits;
	class_data  m_class_data;
};


template<typename T, typename Enable = void>
struct FGetSizeOf
{
	inline constexpr static size_t Value()
	{
		return sizeof(T);
	}
};

/////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct FGetSizeOf<T, EnableIf<std::is_same<T, void>::value || std::is_function<T>::value>>
{
	inline constexpr static size_t Value()
	{
		return 0;
	}
};


template<typename _Ty, bool = std::is_same<_Ty, typename FRawType<_Ty>::type >::value>
struct FRawTypeInfo
{
	static inline FType GetType() noexcept
	{
		return GetInvalidType(); // we have to return an empty type, so we can stop the recursion
	}
};

template<typename _Ty, bool = IsWrapper<_Ty>::value>
struct FWrapperTypeInfo
{
	static inline FType GetType() noexcept
	{
		return FType::Get<WrapperMapper<_Ty>>();
	}
};

template<typename _Ty>
struct FWrapperTypeInfo<_Ty, false>
{
	static inline FType GetType() noexcept
	{
		return GetInvalidType();
	}
};

template<typename _Ty, bool = std::is_array<T>::value>
struct FArrayRawType
{
	static inline type get_type() noexcept
	{
		return FType::Get<RawArrayType<_Ty>>();
	}
};

template<typename _Ty>
struct FArrayRawType<_Ty, false>
{
	static inline FType GetType() noexcept
	{
		return GetInvalidType();  // we have to return an empty type, so we can stop the recursion
	}
};

template<typename _Ty>
TUniquePtr<FTypeData> MakeTypeData()
{
	auto obj = TUniquePtr<FTypeData>
		(
			new FTypeData
			{
				FRawTypeInfo<_Ty>::GetType()._typeData, FWrapperTypeInfo<_Ty>::GetType()._typeData,
				FArrayRawType<_Ty>::GetType()._typeData,

				GetTypeName<T>().CString(), GetTypeName<T>(),

				FGetSizeOf<T>::Value(),
				PointerCount<T>::value,

				&create_variant_func<T>::create_variant,
				&base_classes<T>::get_types,
				nullptr,
				&get_metadata_func_impl<T>,
				get_create_wrapper_func<T>(),

				nullptr,
				true,
				type_trait_value{ TYPE_TRAIT_TO_BITSET_VALUE(is_class) |
									TYPE_TRAIT_TO_BITSET_VALUE(is_enum) |
									TYPE_TRAIT_TO_BITSET_VALUE(is_array) |
									TYPE_TRAIT_TO_BITSET_VALUE(is_pointer) |
									TYPE_TRAIT_TO_BITSET_VALUE(is_arithmetic) |
									TYPE_TRAIT_TO_BITSET_VALUE_2(is_function_ptr, is_function_pointer) |
									TYPE_TRAIT_TO_BITSET_VALUE(is_member_object_pointer) |
									TYPE_TRAIT_TO_BITSET_VALUE(is_member_function_pointer) |
									TYPE_TRAIT_TO_BITSET_VALUE_2(::rttr::detail::is_associative_container, is_associative_container) |
									TYPE_TRAIT_TO_BITSET_VALUE_2(::rttr::detail::is_sequential_container, is_sequential_container) |
									TYPE_TRAIT_TO_BITSET_VALUE_2(::rttr::detail::template_type_trait, is_template_instantiation)
								},
				class_data(get_most_derived_info_func<T>(), template_type_trait<T>::get_template_arguments())
			}
	);
	return obj;
}
}
}