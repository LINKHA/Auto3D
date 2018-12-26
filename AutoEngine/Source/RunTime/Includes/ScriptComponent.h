#pragma once
#include "Component.h"

namespace Auto3D {
class ScriptComponent :public Component
{
	REGISTER_OBJECT_ABSTRACT_CLASS(ScriptComponent, Component)
public:
	explicit ScriptComponent(Ambient* ambient);
	virtual void Init()
	{
		// Get registered node scene ID
		_sceneID = GetNodePtr()->GetSceneID();
	}


	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
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
protected:
	unsigned _sceneID;
};

#if SharedPtrDebug
template<typename T> sharedPtr<T> ScriptComponent::CreateObject()
{ 
	return StaticCast<T>(CreateObject(T::GetClassStringStatic()));
}
#else
template<typename T> T* ScriptComponent::CreateObject()
{
	return static_cast<T*>(CreateObject(T::GetClassStringStatic()));
}
#endif
}

