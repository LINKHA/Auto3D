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
	REGISTER_OBJECT_ABSTRACT_CLASS(BehaviorObject, Object)
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
	SharedPtr<Object> CreateObject(STRING type);
#else
	Object* CreateObject(STRING type);
#endif
	/**
	* @brief : Create object for template
	*/
#if SharedPtrDebug
	template<typename T> SharedPtr<T> CreateObject();
#else
	template<typename T> T* CreateObject();
#endif
	/**
	* @brief : Return is load
	*/
	bool IsEnable() { return _isEnable; }
	/**
	* @brief : Set enable
	*/
	void Enable(bool enable) { _isEnable = enable; }
protected:
	/// is enable
	bool _isEnable;
};




#if SharedPtrDebug
template<typename T> SharedPtr<T> BehaviorObject::CreateObject()
{
	return static_cast<T>(CreateObject(T::GetClassStringStatic()));
}
#else
template<typename T> T* BehaviorObject::CreateObject()
{
	return static_cast<T*>(CreateObject(T::GetClassStringStatic()));
}
#endif
}