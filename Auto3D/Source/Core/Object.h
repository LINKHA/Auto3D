#pragma once
#include "AutoConfig.h"

namespace Auto3D
{
#define DECLARE_RTTR_BASE_CLASS(_This) \
	/*The base Object does not need to specify a parent class*/\
	RTTR_ENABLE() \
	/*Reflected private tag*/\
	RTTR_REGISTRATION_FRIEND 

#define DECLARE_RTTR_CLASS(_This,_Base) \
	/*The base Object does not need to specify a parent class*/\
	RTTR_ENABLE(_Base) \
	/*Reflected private tag*/\
	RTTR_REGISTRATION_FRIEND 

#define DECLARE_CLASS(_This,_Base) \
public: \
	_This& operator=(_This&&) = delete;   \
	_This& operator=(const _This&)= delete;  \
	using This = _This;\
	using Super = _Base;\
	DECLARE_RTTR_CLASS(_This, _Base) \
public:

#define REGISTER_CALSS_IMP(_Class) \
	using namespace rttr;\
	registration::class_<_Class>(#_Class)

/// Base class for objects.
class AUTO_API AObject
{
public:
	/// Structure
	AObject() = default;
	/// Destructor
	virtual ~AObject() = default;

	//FVariant _variant;
};

}
