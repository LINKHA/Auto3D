#pragma once
#include "Auto.h"
AUTO_BEGIN
struct RefCount
{
	RefCount() :
		refs(0),
		weakRefs(0)
	{
	}
	~RefCount()
	{
		// Set reference counts below zero to fire asserts if this object is still accessed
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
	virtual ~RefCounted();
	void AddRef();
	void ReleaseRef();
	int Refs() const;
	int WeakRefs() const;

	RefCount* RefCountPtr() { return _refCount; }

private:
	/// Prevent copy construction.
	RefCounted(const RefCounted& rhs);
	/// Prevent assignment.
	RefCounted& operator =(const RefCounted& rhs);

	/// Pointer to the reference count structure.
	RefCount* _refCount;
};
AUTO_END