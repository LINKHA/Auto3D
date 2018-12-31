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
	explicit BehaviorObject(SharedPtr<Ambient> ambient);

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw() {}

	/**
	* @brief : Create object for type name
	*/
	SharedPtr<Object> CreateObject(STRING type);
	/**
	* @brief : Create object for template
	*/
	template<typename T> SharedPtr<T> CreateObject();
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

template<typename T> SharedPtr<T> BehaviorObject::CreateObject()
{
	return StaticCast<T>(CreateObject(T::GetClassStringStatic()));
}

}