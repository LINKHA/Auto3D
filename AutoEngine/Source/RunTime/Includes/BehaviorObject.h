#pragma once
#include "Object.h"
#include "Ambient.h"

namespace Auto3D {

enum class RunMode
{
	Default = -1,
	Awake,
	Start,
	Update,
	FixUpdate,
	Finish,
	Draw,
};

class BehaviorObject : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(BehaviorObject, Object);
	DECLARE_OBJECT_SERIALIZE(BehaviorObject);
public:
	explicit BehaviorObject(Ambient* ambient);

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw() {}
	/**
	* @brief : Create object for type name
	*/
#if SharedPtrDebug
	sharedPtr<Object> CreateObject(STRING type);
#else
	Object* CreateObject(STRING type);
#endif
	/**
	* @brief : Create object for template
	*/
#if SharedPtrDebug
	template<typename T> sharedPtr<T> CreateObject();
#else
	template<typename T> T* CreateObject();
#endif
};
#if SharedPtrDebug
template<typename T> sharedPtr<T> ScriptComponent::CreateObject()
{
	return StaticCast<T>(CreateObject(T::GetClassStringStatic()));
}
#else
template<typename T> T* BehaviorObject::CreateObject()
{
	return static_cast<T*>(CreateObject(T::GetClassStringStatic()));
}
#endif
}