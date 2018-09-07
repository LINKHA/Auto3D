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
	Object* CreateObject(_String type);
	/**
	* @brief : Create object for template
	*/
	template<typename T> T* CreateObject();
};

template<typename T> T* ScriptComponent::CreateObject(){ return static_cast<T*>(CreateObject(T::GetClassStringStatic())); }

}

