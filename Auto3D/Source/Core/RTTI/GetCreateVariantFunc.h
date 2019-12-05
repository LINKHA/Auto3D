#pragma once
#include "Core/RTTI/Variant.h"
#include "Core/RTTI/Argument.h"

namespace Auto3D
{
using VariantCreateFunc = FVariant(*)(const FArgument&);

template<typename _Ty>
struct FCreateVariantPolicy
{
	static FVariant CreateVariant(const FArgument& data)
	{
		return data.GetValue<_Ty>();
	}
};

struct FCreateInvalidVariantPolicy
{
	static FVariant CreateVariant(const FArgument& data)
	{
		return FVariant();
	}
};


}