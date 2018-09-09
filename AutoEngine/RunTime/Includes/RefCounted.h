#pragma once
#include "Auto.h"
namespace Auto3D {


struct RefCount
{
	/**
	* @brief : Construct
	*/
	RefCount() :
		refs(0),
		weakRefs(0)
	{}
	/**
	* @brief : Destruct
	*/
	~RefCount()
	{
		// Set reference counts below zero to fire Asserts if this object is still accessed
		refs = -1;
		weakRefs = -1;
	}
	/// Reference count. If below zero, the object has been destroyed.
	int refs;
	/// Weak reference count.
	int weakRefs;
};


/**
* Base class for intrusively reference-counted objects. 
* These are noncopyable and non-assignable.
*/
class RefCounted
{
public:
	/**
	* @brief : Construct. Allocate the reference count 
	*		structure and set an initial self weak reference.
	*/
	RefCounted();
	/**
	* @brief : Destruct. Mark as expired and also delete the reference 
	*		count structure if no outside weak references exist.
	*/
	virtual ~RefCounted();
	/**
	* @brief : Prevent copy construction.
	*/
	RefCounted(const RefCounted& rhs) = delete;
	/**
	* @brief : Prevent assignment.
	*/
	RefCounted& operator =(const RefCounted& rhs) = delete;

	/**
	* @brief : Increment reference count. Can also be called outside 
	*		of a SharedPtr for traditional reference counting.
	*/
	void AddRef();
	/**
	* @brief : Decrement reference count and delete self if no more 
	*		references. Can also be called outside of a SharedPtr for 
	*		traditional reference counting.
	*/
	void ReleaseRef();
	/**
	* @brief : Return reference count.
	*/
	int Refs() const;
	/**
	* @brief : Return weak reference count.
	*/
	int WeakRefs() const;
	/**
	* @brief : Return pointer to the reference count structure.
	*/
	RefCount* RefCountPtr() { return _refCount; }
	
private:
	/// Pointer to the reference count structure.
	RefCount* _refCount;
};
}