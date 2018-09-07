#pragma once
#include "GameObject.h"

namespace Auto3D {
class ScriptComponent :public Component
{
	REGISTER_DERIVED_ABSTRACT_CLASS(ScriptComponent, Component);
	DECLARE_OBJECT_SERIALIZE(ScriptComponent);
public:
	explicit ScriptComponent(Ambient* ambient);

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	/**
	* @brief : Create object for type name
	*/
	SharedPtr<Object> CreateObject(_String type);
	/**
	* @brief : Create object for template
	*/
	template<typename T> SharedPtr<T> CreateObject();
};

template<typename T> SharedPtr<T> ScriptComponent::CreateObject()
{ 
	return StaticCast<T>(CreateObject(T::GetClassStringStatic()));
}

}

