#pragma once
#include "Auto.h"
#include "LogAssert.h"
#include "ClassID.h"
#include "stl_use.h"
#include "ObjectDefines.h"

AUTO_BEGIN

class Object 
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

	Object();

	virtual int GetClassID() const						{ return ClassID(Object); }
	void SetClassID(ClassIDType classId)				{ _classID = classId; }
	void SetInstanceID(int inID)						{ _instanceID = inID; }
	Int32 GetInstanceID() const							{ assert(_instanceID != 0); return _instanceID; }
	

	virtual char const* GetName() const					{ return ""; };
	virtual void SetName(char const* name)				{ }
	

	static int GetClassIDStatic()						{ return ClassID(Object); }
	static const char* GetClassStringStatic()			{ return "Object"; }
	static const char* GetSharedPtrTypeString()			{ return "SharedPtr<Object>"; }
	static bool IsAbstract()							{ return true; }

	template<class TransferFunction>
	void Transfer(TransferFunction& transfer);

	const std::string& Object::GetClassName()const;
	static const std::string& Object::ClassIDToString(int ID);

	static int StringToClassID(const std::string& classString);
	static int StringToClassID(const char* classString);
private:
	Int32 _instanceID;
	Int32 _classID;

};


AUTO_END
