#pragma once
#include "Auto.h"
#include "LogAssert.h"
#include "ClassID.h"
#include "stl_use.h"
#include "ObjectDefines.h"
#include "RefCounted.h"

namespace Auto3D {
class Ambient;

///!!!Temp
template <class T> inline void swap(T& first, T& second)
{
	T temp = first;
	first = second;
	second = temp;
}

template <class T> class SharedPtr
{
public:
	SharedPtr() noexcept
		: _ptr(nullptr)
	{}

	SharedPtr(const SharedPtr<T>& rhs) noexcept
		: _ptr(rhs._ptr)
	{
		AddRef();
	}

	template <class U> SharedPtr(const SharedPtr<U>& rhs) noexcept
		: _ptr(rhs._ptr)
	{
		AddRef();
	}

	explicit SharedPtr(T* ptr) noexcept :
		_ptr(ptr)
	{
		AddRef();
	}
	virtual ~SharedPtr()noexcept
	{
		ReleaseRef();
	}

	SharedPtr<T>& operator =(T* ptr)
	{
		if (this->_ptr == ptr)
			return *this;

		SharedPtr<T> copy(ptr);
		Swap(copy);

		return *this;
	}

	T* operator ->() const { Assert(_ptr); return _ptr; }

	T& operator *() const { Assert(_ptr); return *_ptr; }

	T& operator [](int index) { Assert(_ptr); return _ptr[index]; }

	template <class U> bool operator <(const SharedPtr<U>& rhs) const { return _ptr < rhs._ptr; }

	template <class U> bool operator ==(const SharedPtr<U>& rhs) const { return _ptr == rhs._ptr; }

	template <class U> bool operator !=(const SharedPtr<U>& rhs) const { return _ptr != rhs._ptr; }

	operator T*() const { return _ptr; }

	void Swap(SharedPtr& rhs) { swap(_ptr, rhs._ptr); }

	void Reset() { ReleaseRef(); }

	T* Detach()
	{
		T* ptr = _ptr;
		if (_ptr)
		{
			RefCount* refCount = RefCountPtr();
			++refCount->refs; // 2 refs
			Reset(); // 1 ref
			--refCount->refs; // 0 refs
		}
		return ptr;
	}

	template <class U> void StaticCast(const SharedPtr<U>& rhs)
	{
		SharedPtr<T> copy(static_cast<T*>(rhs->Get()));
		Swap(copy);
	}
	template <class U> void DynamicCast(const SharedPtr<U>& rhs)
	{
		SharedPtr<T> copy(dynamic_cast<T*>(rhs.Get()));
		Swap(copy);
	}

	bool Null() const { return _ptr == 0; }

	bool NotNull() const { return _ptr != nullptr; }

	T* Get() const { return _ptr; }

	int Refs() const { return _ptr ? _ptr->Refs() : 0; }

private:
	template <class U> friend class SharedPtr;

	void AddRef()
	{
		if (_ptr)
			_ptr->AddRef();
	}
	void ReleaseRef()
	{
		if (_ptr)
		{
			_ptr->ReleaseRef();
			_ptr = nullptr;
		}
	}

	/// Pointer to the object.
	T * _ptr;

};



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
	Int32 GetInstanceID() const							{ Assert(_instanceID != 0); return _instanceID; }


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
	
protected:
	Ambient* _ambient;
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
		Assert(_ambient);
	}

	Ambient* _ambient;
};

}
