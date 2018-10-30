#pragma once
#include "Auto.h"
#include "LogAssert.h"
#include "ClassID.h"
#include "AutoSTL.h"
#include "ObjectDefines.h"
#include "RefCounted.h"
#include "SharedPtr.h"
#include "RTTI.h"

namespace Auto3D {

class Ambient;

/**
* Base object for all regisiter class
*/
class Object : public RefCounted
{
protected:
	virtual ~Object() { }
public:
	explicit Object(Ambient* ambient);
	///////////////////////////////////////////////////////////////////////////
	// @brief : Register class function
	virtual int GetClassID() { return ClassID(Object); }
	virtual bool IsAbstract() { return true; }
	virtual char* GetClassCstrName() { return "Object"; }
	virtual STRING GetClassString() { return Khs("Object"); }
	virtual Auto3D::RTTI* GetRTTI() { return GetRTTIStatic(); }
	static int GetClassIDStatic() { return ClassID(Object); }
	static bool IsAbstractStatic() { return true; }
	static char* GetClassCstrNameStatic() { return "Object"; }
	static STRING GetClassStringStatic() { return Khs("Object"); }
	static Auto3D::RTTI* GetRTTIStatic() { return nullptr; }
	//////////////////////////////////////////////////////////////////////////
	
	/**
	* @brief : Return execution ambient.
	*/
	Ambient* GetAmbient() const { return _ambient; }
	/**
	* @brief : Return subsystem by type.
	*/
	Object* GetSubSystem(STRING type)const;
	/**
	* @brief : Template version of returning a subsystem.
	*/
	template<typename T> T* GetSubSystem() const;
	/**
	* @brief : Get object attach
	*/
	const STRING GetAttach();


protected:
	Ambient* _ambient;
};


template<typename T> T* Object::GetSubSystem() const
{
	return static_cast<T*>(Object::GetSubSystem(T::GetClassStringStatic()));
}


}
