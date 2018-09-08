#pragma once
#include "Object.h"

namespace Auto3D {

class StateVessel : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(StateVessel, Object);
	DECLARE_OBJECT_SERIALIZE(StateVessel);
public:
	explicit StateVessel(Ambient* ambient);



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
	SharedPtr<Object> CreateObject(_String type);
#else
	Object* CreateObject(_String type);
#endif
	/**
	* @brief : Create object for template
	*/
#if SharedPtrDebug
	template<typename T> SharedPtr<T> CreateObject();
#else
	template<typename T> T* CreateObject();
#endif
};
#if SharedPtrDebug
template<typename T> SharedPtr<T> ScriptComponent::CreateObject()
{
	return StaticCast<T>(CreateObject(T::GetClassStringStatic()));
}
#else
template<typename T> T* StateVessel::CreateObject()
{
	return static_cast<T*>(CreateObject(T::GetClassStringStatic()));
}
#endif
}