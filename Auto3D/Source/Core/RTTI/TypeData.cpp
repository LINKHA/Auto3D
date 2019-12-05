#include "TypeData.h"
#include "Core/RTTI/Type.h"

namespace Auto3D
{
inline static FType GetInvalidType() noexcept
{
	return CreateType(nullptr);
}

static FTypeData& GetInvalidTypeDataImpl() noexcept
{
	static FTypeData instance{ nullptr, nullptr,
								nullptr,
								FString(), FString(),
								0, 0,
								&create_invalid_variant_policy::create_variant,
								&base_classes<void>::get_types,
								nullptr,
								nullptr,
								get_create_wrapper_func<void>(),
								nullptr,
								false,
								type_trait_value{0},
								class_data(nullptr, std::vector<type>())
	};

	instance.raw_type_data = &instance;
	instance.wrapped_type = &instance;
	instance.array_raw_type = &instance;

	return instance;
}

/////////////////////////////////////////////////////////////////////////////////////////

FTypeData* GetInvalidTypeData() noexcept
{
	static auto instance = &GetInvalidTypeDataImpl();
	return instance;
}

}