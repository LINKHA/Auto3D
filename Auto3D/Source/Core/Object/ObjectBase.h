#pragma once
#include "AutoConfig.h"
#include "Core/Object/Class.h"

namespace Auto3D
{
class AUTO_API AObjectBase
{

};

/// Base class for deferred native class registration
struct FFieldCompiledInInfo
{
	FFieldCompiledInInfo(size_t inClassSize)
		: _size(inClassSize)
		, _oldClass(nullptr)
		, _bHasChanged(false)
	{
	}

	/// Registers the native class (constructs a UClass object).
	virtual AClass* Register() const = 0;

	/// Size of the class.
	size_t _size;
	/// Old UClass object.
	AClass* _oldClass;
	/// True if this class has changed after hot-reload (or new class).
	bool _bHasChanged;
};
/// Adds a class to deferred registration queue.
AUTO_API void UClassCompiledInDefer(FFieldCompiledInInfo* Class, size_t ClassSize);


/**
 * Specialized version of the deferred class registration structure.
 */
template <typename _TClass>
struct TClassCompiledInDefer : public FFieldCompiledInInfo
{
	TClassCompiledInDefer(const char* inName, size_t inClassSize)
		: FFieldCompiledInInfo(inClassSize)
	{
		UClassCompiledInDefer(this, inName, inClassSize);
	}
	virtual AClass* Register() const override
	{
		return _TClass::StaticClass();
	}
};
}