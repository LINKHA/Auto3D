#pragma once
#include "Auto.h"
namespace Auto3D {
struct RefCount
{
	RefCount() :
		refs(0),
		weakRefs(0)
	{
	}
	~RefCount()
	{
		// Set reference counts below zero to fire Asserts if this object is still accessed
		refs = -1;
		weakRefs = -1;
	}
	/// Reference count. If below zero, the object has been destroyed.
	int refs;
	int weakRefs;
};

/// Base class for intrusively reference-counted objects. These are noncopyable and non-assignable.
class RefCounted
{
public:
	RefCounted();
	//RefCounted(const RefCounted& rhs) = delete;
	//RefCounted& operator =(const RefCounted& rhs) = delete;
	virtual ~RefCounted();
	void AddRef();
	void ReleaseRef();
	int Refs() const;
	int WeakRefs() const;
	RefCount* RefCountPtr() { return _refCount; }
private:
	/// Pointer to the reference count structure.
	RefCount* _refCount;
};
}