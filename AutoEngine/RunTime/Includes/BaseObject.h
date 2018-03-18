#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_
#include "Auto.h"


AUTO_BEGIN
class Object;

template<class T>
class PPtr
{
	UInt32 m_InstanceID;
	static const char* GetTypeString();//{ return typeid(T).name(); }
public:
	PPtr(int instanceID) { m_InstanceID = instanceID; }
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

	operator T* () const;
	T* operator-> ()const;
	T& operator* ()const;

	bool IsNull()const;
	bool IsValid()const;
};

class Object 
{
protected:
	virtual ~Object();
public:

	Object();

	UInt32 GetInstanceID() const						{ AssertIf(m_InstanceID == 0); return m_InstanceID; }

	virtual char const* GetName() const					{ return ""; };
	virtual void SetName(char const* name){}

	static const char* GetPPtrTypeString()				{ return "PPtr<Object>"; }
	static const char* GetClassStringStatic()			{ return "Object"; }
private:
	UInt32 m_InstanceID;

	//Object(const Object& o);							// Disallow copy constructor




};
AUTO_END
#endif // !BASE_OBJECT_H_
