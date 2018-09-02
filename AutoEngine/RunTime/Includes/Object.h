#pragma once
#include "Auto.h"
#include "LogAssert.h"
#include "ClassID.h"
#include "stl_use.h"
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
	
	Object() {}
	explicit Object(Ambient* ambient);
	///////////////////////////////////////////////////////////////////////////
	// @brief : Register class function
	virtual int GetClassID() { return ClassID(Object); }
	virtual bool IsAbstract() { return true; }
	virtual const char* GetClassName() { return "Object"; }
	virtual const _String GetClassString() { return "Object"; }
	virtual const Auto3D::RTTI* GetRTTI() { return GetRTTIStatic(); }
	static int GetClassIDStatic() { return ClassID(Object); }
	static bool IsAbstractStatic() { return true; }
	static const char* GetClassNameStatic() { return "Object"; }
	static const _String GetClassStringStatic() { return "Object"; }
	static const Auto3D::RTTI* GetRTTIStatic() { return nullptr; }
	//////////////////////////////////////////////////////////////////////////
	
	/**
	* @brief : Return execution ambient.
	*/
	Ambient* GetAmbient() const { return _ambient; }
	/**
	* @brief : Return subsystem by type.
	*/
	Object* GetSubSystem(_String type)const;
	/**
	* @brief : Template version of returning a subsystem.
	*/
	template<typename T> T* GetSubSystem() const;
	/**
	* @brief : Get object attach
	*/
	const _String& GetAttach() const;

protected:
	Ambient* _ambient;
};

template<typename T> 
T* Object::GetSubSystem() const 
{ 
	return static_cast<T*>(Object::GetSubSystem(T::GetClassStringStatic())); 
}


}
