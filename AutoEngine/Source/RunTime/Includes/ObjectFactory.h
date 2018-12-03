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
#if SharedPtrDebug
	virtual sharedPtr<Object> CreateObject() = 0;
#else
	virtual Object* CreateObject() = 0;
#endif
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
	const STRING GetClassString() { return _RTTI->GetClassString(); }
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

template<typename _Ty> class ObjectFactoryImpl : public ObjectFactory
{
public:
	/// Construct
	explicit ObjectFactoryImpl(Ambient* ambient) :
		ObjectFactory(ambient)
	{
		_RTTI = _Ty::GetRTTIStatic();
	}
	/**
	* @brief : Create an object of the specific type.
	*/
#if SharedPtrDebug
	sharedPtr<Object>  CreateObject() override { return sharedPtr<Object>(new _Ty(_ambient)); }
#else
	Object* CreateObject() override { return new _Ty(_ambient); }
#endif
};


}