#pragma once
#include "Auto.h"
#include "LogAssert.h"
#include "ClassID.h"
#include "stl_use.h"
#include "ObjectDefines.h"
#include "RefCounted.h"
#include "SharedPtr.h"

namespace Auto3D {


class Ambient;
class Object : public RefCounted
{
protected:
	virtual ~Object()									{ }
public:
	struct RTTI
	{
		RTTI* base;// super rtti class
		//Object::FactoryFunction* factory;// the factory function of the class
		int classID;
		std::string className;
		int size;// sizeof (Class)
		bool isAbstract;// is the class Abstract?
	};
	Object() {}
	explicit Object(Ambient* ambient);

	
	void SetClassID(ClassIDType classId)				{ _classID = classId; }
	void SetInstanceID(int inID)						{ _instanceID = inID; }
	Int32 GetInstanceID() const							{ Assert(_instanceID != 0); return _instanceID; }

	///////////////////////////////////////////////////////////////////////////
	// @brief : Static regisiter
	static int GetClassIDStatic() { return ClassID(Object); }
	static bool IsAbstractStatic()	{ return true; }
	static const char* GetClassStringStatic() { return "Object"; }
	static const char* GetSharedPtrTypeStringStatic() { return "SharedPtr<Object>"; }
	//////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// @brief : Virtual regisiter
	virtual int GetClassIDVirtual() const				{ return ClassID(Object); }
	virtual const char* GetClassStringVirtual()			{ return "Object"; }
	virtual const char* GetSharedPtrTypeStringVirtual() { return "SharedPtr<Object>"; }
	//////////////////////////////////////////////////////////////////////////
	const std::string& Object::GetClassName()const;
	static const std::string& Object::ClassIDToString(int ID);

	static int StringToClassID(const std::string& classString);
	static int StringToClassID(const char* classString);

	Object* GetSubSystem(_String type)const;
	template<typename T> T* GetSubSystem() const;
	
protected:
	Ambient* _ambient;
private:
	Int32 _instanceID;
	Int32 _classID;
};

template <class T> 
T* Object::GetSubSystem() const 
{ 
	return static_cast<T*>(Object::GetSubSystem(T::GetClassStringStatic())); 
}

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
	///////////////////////////////////////////////////////////////////////////
	// @brief : Static regisiter
	static int GetClassIDStatic() { return ClassID(ObjectFactory); }
	static const char* GetClassStringStatic() { return "ObjectFactory"; }
	static const char* GetSharedPtrTypeStringStatic() { return "SharedPtr<ObjectFactory>"; }
	static bool IsAbstractStatic() { return true; }
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// @brief : Static regisiter
	virtual int GetClassIDVirtual() const { return ClassID(ObjectFactory); }
	virtual const char* GetClassStringVirtual() { return "ObjectFactory"; }
	virtual const char* GetSharedPtrTypeStringVirtual() { return "SharedPtr<ObjectFactory>"; }
	///////////////////////////////////////////////////////////////////////////
protected:
	Ambient* _ambient;
};

template <class T> class ObjectFactoryImpl : public ObjectFactory
{
public:
	/// Construct.
	explicit ObjectFactoryImpl(Ambient* ambient) :
		ObjectFactory(ambient)
	{
	}
	///////////////////////////////////////////////////////////////////////////
	// @brief : Static regisiter
	static int GetClassIDStatic() { return T::GetClassIDStatic(); }
	static const char* GetClassStringStatic() { return T::GetClassStringStatic(); }
	static const char* GetSharedPtrTypeStringStatic() { return  T::GetSharedPtrTypeString(); }
	static bool IsAbstractStatic() { return T::IsAbstractStatic(); }
	///////////////////////////////////////////////////////////////////////////

	/// Create an object of the specific type.
	SharedPtr<Object> CreateObject() override { return SharedPtr<Object>(new T(_ambient)); }
};



}
