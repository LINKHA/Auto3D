#pragma once
#include "SharedPtr.h"
#include "RefCounted.h"
#include "ClassID.h"
#include "RTTI.h"

namespace Auto3D {

class Ambient;
class Object;

class ObjectFactory : public RefCounted
{
public:
	explicit ObjectFactory(Ambient* ambient)
		:_ambient(ambient)
	{
		Assert(_ambient);
	}
	/**
	* @brief : Create an object.Implemented in templated subclasses.
	*/
	virtual SharedPtr<Object> CreateObject() = 0;
	/**
	* @brief : Return execution ambient
	*/
	Ambient* GetAmbient() const { return _ambient; }
	/**
	* @brief : Retrun template Class Name for c str
	*/
	const char* GetClassCstrName() { return _RTTI->GetClassCstrName(); }
	/**
	* @brief : Retrun template Class Name for string
	*/
	const _String GetClassString() { return _RTTI->GetClassString(); }
	/**
	* @brief : Retrun template Class RTTI
	*/
	const Auto3D::RTTI* GetRTTI() { return _RTTI; }
protected:
	///Execution ambient
	Ambient* _ambient;
	///RTTI info
	RTTI* _RTTI{};
};

template<typename T> class ObjectFactoryImpl : public ObjectFactory
{
public:
	/// Construct.
	explicit ObjectFactoryImpl(Ambient* ambient) :
		ObjectFactory(ambient)
	{
		_RTTI = T::GetRTTIStatic();
	}
	/**
	* @brief : Create an object of the specific type.
	*/
	SharedPtr<Object>  CreateObject() override { return SharedPtr<Object>(new T(_ambient)); }
};


}