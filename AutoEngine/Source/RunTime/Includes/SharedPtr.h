#pragma once
#include "LogAssert.h"
#include "KhSTL/Algorithm/TypeAlgorithm.h"
namespace Auto3D {


template<typename T> class sharedPtr
{
public:
	sharedPtr() noexcept
		: _ptr(nullptr)
	{}

	sharedPtr(const sharedPtr<T>& rhs) noexcept
		: _ptr(rhs._ptr)
	{
		AddRef();
	}

	template <class U> sharedPtr(const sharedPtr<U>& rhs) noexcept
		: _ptr(rhs._ptr)
	{
		AddRef();
	}

	explicit sharedPtr(T* ptr) noexcept :
		_ptr(ptr)
	{
		AddRef();
	}
	virtual ~sharedPtr()noexcept
	{
		ReleaseRef();
	}

	sharedPtr<T>& operator =(T* ptr)
	{
		if (this->_ptr == ptr)
			return *this;

		sharedPtr<T> copy(ptr);
		Swap(copy);

		return *this;
	}

	T* operator ->() const { Assert(_ptr); return _ptr; }

	T& operator *() const { Assert(_ptr); return *_ptr; }

	T& operator [](int index) { Assert(_ptr); return _ptr[index]; }

	template <class U> bool operator <(const sharedPtr<U>& rhs) const { return _ptr < rhs._ptr; }

	template <class U> bool operator ==(const sharedPtr<U>& rhs) const { return _ptr == rhs._ptr; }

	template <class U> bool operator !=(const sharedPtr<U>& rhs) const { return _ptr != rhs._ptr; }

	operator T*() const { return _ptr; }

	void Swap(sharedPtr& rhs) { KhSTL::Swap(_ptr, rhs._ptr); }

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

	template <class U> void StaticCast(const sharedPtr<U>& rhs)
	{
		sharedPtr<T> copy(static_cast<T*>(rhs.Get()));
		Swap(copy);
	}
	template <class U> void DynamicCast(const sharedPtr<U>& rhs)
	{
		sharedPtr<T> copy(dynamic_cast<T*>(rhs.Get()));
		Swap(copy);
	}

	bool Null() const { return _ptr == 0; }

	bool NotNull() const { return _ptr != nullptr; }

	T* Get() const { return _ptr; }

	int Refs() const { return _ptr ? _ptr->Refs() : 0; }

	long UseCount()const { return RefCountPtr()->refs; }

private:
	template <class U> friend class sharedPtr;

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
	T* _ptr;

};

/**
* @brief : Perform a static cast from one shared pointer type to another.
*/
template <class T, class U> sharedPtr<T> StaticCast(const sharedPtr<U>& ptr)
{
	sharedPtr<T> ret;
	ret.StaticCast(ptr);
	return ret;
}

/**
* @brief : Perform a dynamic cast from one weak pointer type to another.
*/
template <class T, class U> sharedPtr<T> DynamicCast(const sharedPtr<U>& ptr)
{
	sharedPtr<T> ret;
	ret.DynamicCast(ptr);
	return ret;
}

}