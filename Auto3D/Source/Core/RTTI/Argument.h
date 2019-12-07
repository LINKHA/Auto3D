#pragma once
#include "AutoConfig.h"
#include "Core/RTTI/Type.h"

namespace Auto3D
{
namespace RTTI
{

class AUTO_API FArgument
{
public:
	inline FArgument() noexcept;
	inline FArgument(FArgument&& arg) noexcept;
	inline FArgument(const FArgument& other) noexcept;

private:
	const void* _data;
	const FVariant* _variant;
	const FType _type;
};

inline FArgument::FArgument() noexcept : _data(nullptr), _variant(nullptr), _type(GetInvalidType()) {}

/////////////////////////////////////////////////////////////////////////////////////////

inline FArgument::FArgument(FArgument&& arg) noexcept : _data(arg._data), _variant(arg._variant), _type(arg._type) {}

/////////////////////////////////////////////////////////////////////////////////////////

inline FArgument::FArgument(const FArgument& other) noexcept : _data(other._data), _variant(other._variant), _type(other._type) {}
}
}