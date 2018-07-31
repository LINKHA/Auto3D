#pragma once
#include "Auto.h"
#include "stl_use.h"
namespace Auto3D {

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

	T* operator ->() const { assert(_ptr); return _ptr; }

	T& operator *() const { assert(_ptr); return *_ptr; }

	T& operator [](int index) { assert(_ptr); return _ptr[index]; }

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

}