#pragma once
#include "Core/Object/ObjectBase.h"
#include "Core/Object/Class.h"
namespace Auto3D
{

#define DECLARE_CLASS(_TClass,_SuperClass) \
private: \
	friend struct Z_Construct##_TClass##_Statics; \
	static AClass* GetPrivateStaticClass(); \
public: \
	_TClass& operator=(_TClass&&) = delete;   \
	_TClass& operator=(const _TClass&) = delete;  \
	/** Typedef for the base class ({{ typedef-type }}) */ \
	typedef _SuperClass Super;\
	/** Typedef for {{ typedef-type }}. */ \
	typedef _TClass This;\
	/** Returns a UClass object representing this class at runtime */ \
	inline static AClass* StaticClass() \
	{ \
		return GetPrivateStaticClass(); \
	} \


#define IMPLEMENT_CLASS(_TClass) \
	static TClassCompiledInDefer<_TClass> AutoInitialize##_TClass(#_TClass, sizeof(_TClass)); \
	AClass* _TClass::GetPrivateStaticClass() \
	{ \
		static AClass* privateStaticClass = NULL; \
		if (!privateStaticClass) \
		{ \
			/* this could be handled with templates, but we want it external to avoid code bloat */ \
			GetPrivateStaticClassBody( \
			); \
		} \
		return privateStaticClass; \
	}

}

