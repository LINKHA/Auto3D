#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_
#include "Auto.h"
#include "LogAssert.h"
#include "ClassID.h"
#include "stl_use.h"
#include "ObjectDefines.h"

AUTO_BEGIN

class Object;

template<class T>
class PPtr
{
	UInt32 m_InstanceID;
	static const char* GetTypeString();
public:
	explicit PPtr(int instanceID) { m_InstanceID = instanceID; }
	PPtr(const T* o) { AssignObject(o); }
	PPtr(const PPtr<T>& o) { m_InstanceID = o.m_InstanceID; }
	PPtr() { m_InstanceID = 0; }
	
	void AssignObject(const Object* o);
	void SetInstanceID(int instanceID) { m_InstanceID = instanceID; }
	UInt32 GetInstanceID() const { return m_InstanceID; }

	PPtr& operator= (const T* o) { AssignObject(o); return *this; }
	PPtr& operator= (const PPtr<T>& o) { m_InstanceID = o.m_InstanceID; return *this; }

	bool operator< (const PPtr& p)const { return m_InstanceID < p.m_InstanceID; }
	bool operator== (const PPtr& p)const { return m_InstanceID == p.m_InstanceID; }
	bool operator!= (const PPtr& p)const { return m_InstanceID != p.m_InstanceID; }

	// MSVC gets confused whether it should use operator bool(), or operator T* with implicit
	// comparison to NULL. So we add explicit functions and use them instead.
	bool IsNull() const;
	bool IsValid() const;

	operator T* () const;
	T* operator-> ()const;
	T& operator* ()const;
};



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

	ClassIDType GetClassID() const						{ /*AssertIf(m_InstanceID == 0);*/ return (ClassIDType)m_ClassID; }
	void SetClassID(ClassIDType classId)				{ m_ClassID = classId; }
	void SetInstanceID(int inID)						{ m_InstanceID = inID; }
	Int32 GetInstanceID() const							{ AssertIf(m_InstanceID == 0); return m_InstanceID; }
	

	virtual char const* GetName() const					{ return ""; };
	virtual void SetName(char const* name)				{ }
	

	static int GetClassIDStatic()						{ return ClassID(Object); }
	static const char* GetClassStringStatic()			{ return "Object"; }
	static const char* GetPPtrTypeString()				{ return "PPtr<Object>"; }
	static bool IsAbstract()							{ return true; }

	template<class TransferFunction>
	void Transfer(TransferFunction& transfer);

	const std::string& Object::GetClassName()const;
	static const std::string& Object::ClassIDToString(int ID);

	static int StringToClassID(const std::string& classString);
	static int StringToClassID(const char* classString);
private:
	Int32 m_InstanceID;
	Int32 m_ClassID;

};


AUTO_END
#endif // !BASE_OBJECT_H_


