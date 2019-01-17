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
	/**
	* @brief : Construct
	*/
	explicit BehaviorObject(SharedPtr<Ambient> ambient);
	/**
	* @brief : Virtual behavior Awake
	*/
	virtual void Awake() {}
	/**
	* @brief : Virtual behavior Start
	*/
	virtual void Start() {}
	/**
	* @brief : Virtual behavior Update
	*/
	virtual void Update() {}
	/**
	* @brief : Virtual behavior FixUpdate
	*/
	virtual void FixUpdate() {}
	/**
	* @brief : Virtual behavior Finish
	*/
	virtual void Finish() {}
	/**
	* @brief : Virtual behavior Draw
	*/
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