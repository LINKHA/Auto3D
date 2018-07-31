#pragma once
#include "Auto.h"
#include "LogAssert.h"
#include "ClassID.h"
#include "stl_use.h"
#include "ObjectDefines.h"
#include "RefCounted.h"

namespace Auto3D {
class Ambient;
class Object : public RefCounted
{
protected:
	virtual ~Object()									{ }
public:
	struct RTTI
	{
		RTTI*                    base;// super rtti class
		//Object::FactoryFunction* factory;// the factory function of the class
		int                      classID;
		std::string              className;
		int                      size;// sizeof (Class)
		bool                     isAbstract;// is the class Abstract?
	};
	Object() {}
	explicit Object(Ambient* ambient);

	
	void SetClassID(ClassIDType classId)				{ _classID = classId; }
	void SetInstanceID(int inID)						{ _instanceID = inID; }
	Int32 GetInstanceID() const							{ assert(_instanceID != 0); return _instanceID; }


	static int GetClassIDStatic()						{ return ClassID(Object); }
	static const char* GetClassStringStatic()			{ return "Object"; }
	static const char* GetSharedPtrTypeString()			{ return "SharedPtr<Object>"; }
	static bool IsAbstractStatic()						{ return true; }

	static const char* GetTypeString()					{ return GetClassStringStatic(); }\
	virtual int GetClassIDVirtual() const				{ return ClassID(Object); }\
	virtual const char* GetClassStringVirtual()			{ return "Object"; }\
	virtual const char* GetSharedPtrTypeStringVirtual() { return "SharedPtr<Object>"; }



	const std::string& Object::GetClassName()const;
	static const std::string& Object::ClassIDToString(int ID);

	static int StringToClassID(const std::string& classString);
	static int StringToClassID(const char* classString);

	Object* GetSubSystem(_String type)const;
	template<typename T> T* GetSubSystem() const;
	Ambient* _ambient;
protected:
	
private:
	Int32 _instanceID;
	Int32 _classID;
};
template <class T> T* Object::GetSubSystem() const { return static_cast<T*>(Object::GetSubSystem(T::GetClassStringStatic())); }

class ObjectFactory : public RefCounted
{
public:
	explicit ObjectFactory(Ambient* ambient)
		:_ambient(ambient)
	{
		assert(_ambient);
	}

	Ambient* _ambient;
};

}
