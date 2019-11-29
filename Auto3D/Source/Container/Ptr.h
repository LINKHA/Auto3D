#pragma once

#include "AutoConfig.h"


#include <cassert>
#include <cstddef>
#include <utility>

namespace Auto3D
{

class FRefCounted;
template <typename _Ty> class WeakPtr;

/// Reference count structure. Used in both intrusive and non-intrusive reference counting.
struct AUTO_API FRefCount
{
    /// Construct with zero refcounts.
    FRefCount() :
        _refs(0),
        _weakRefs(0),
        _expired(false)
    {
    }

    /// Number of strong references. These keep the object alive.
    unsigned _refs;
    /// Number of weak references.
    unsigned _weakRefs;
    /// Expired flag. The object is no longer safe to access after this is set true.
    bool _expired;
};

/// Base class for intrusively reference counted objects that can be pointed to with SharedPtr and WeakPtr. These are not copy-constructible and not assignable.
class AUTO_API FRefCounted
{
public:
    /// Construct. The reference count is not allocated yet; it will be allocated on demand.
    FRefCounted();

    /// Destruct. If no weak references, destroy also the reference count, else mark it expired.
    virtual ~FRefCounted();
	/// Prevent copy construction.
	FRefCounted(const FRefCounted& rhs) = delete;
	/// Prevent assignment.
	FRefCounted& operator = (const FRefCounted& rhs) = delete;

    /// Add a strong reference. Allocate the reference count structure first if necessary.
    void AddRef();
    /// Release a strong reference. Destroy the object when the last strong reference is gone.
    void ReleaseRef();

    /// Return the number of strong references.
    unsigned Refs() const { return _refCount ? _refCount->_refs : 0; }
    /// Return the number of weak references.
    unsigned WeakRefs() const { return _refCount ? _refCount->_weakRefs : 0; }
    /// Return pointer to the reference count structure. Allocate if not allocated yet.
    FRefCount* RefCountPtr();

private:
   
    /// Reference count structure, allocated on demand.
    FRefCount* _refCount;
};

/// Pointer which holds a strong reference to a FRefCounted subclass and allows shared ownership.
template <typename _Ty> class SharedPtr
{
public:
    /// Construct a null pointer.
    SharedPtr() :
        _ptr(nullptr)
    {
    }
    
    /// Copy-construct.
    SharedPtr(const SharedPtr<_Ty>& ptr) :
        _ptr(nullptr)
    {
        *this = ptr;
    }

    /// Construct from a raw pointer.
    SharedPtr(_Ty* ptr) :
        _ptr(nullptr)
    {
        *this = ptr;
    }
    
    /// Destruct. Release the object reference and destroy the object if was the last strong reference.
    ~SharedPtr()
    {
        Reset();
    }
    
    /// Assign a raw pointer.
    SharedPtr<_Ty>& operator = (_Ty* rhs)
    {
        if (Get() == rhs)
            return *this;

        Reset();
        _ptr = rhs;
        if (_ptr)
            _ptr->AddRef();
        return *this;
    }
    
    /// Assign another shared pointer.
    SharedPtr<_Ty>& operator = (const SharedPtr<_Ty>& rhs)
    {
        if (*this == rhs)
            return *this;

        Reset();
        _ptr = rhs._ptr;
        if (_ptr)
            _ptr->AddRef();
        return *this;
    }
    
    /// Release the object reference and reset to null. Destroy the object if was the last reference.
    void Reset()
    {
        if (_ptr)
        {
            _ptr->ReleaseRef();
            _ptr = nullptr;
        }
    }
    
    /// Perform a static cast from a shared pointer of another type.
    template <typename _Oth> void StaticCast(const SharedPtr<_Oth>& rhs)
    {
        *this = static_cast<_Ty*>(rhs._ptr);
    }

    /// Perform a dynamic cast from a weak pointer of another type.
    template <typename _Oth> void DynamicCast(const WeakPtr<_Oth>& rhs)
    {
        Reset();
        _Ty* rhsObject = dynamic_cast<_Ty*>(rhs.ptr);
        if (rhsObject)
            *this = rhsObject;
    }
    
    /// Test for equality with another shared pointer.
    bool operator == (const SharedPtr<_Ty>& rhs) const { return _ptr == rhs._ptr; }
    /// Test for equality with a raw pointer.
    bool operator == (_Ty* rhs) const { return _ptr == rhs; }
    /// Test for inequality with another shared pointer.
    bool operator != (const SharedPtr<_Ty>& rhs) const { return !(*this == rhs); }
    /// Test for inequality with a raw pointer.
    bool operator != (_Ty* rhs) const { return !(*this == rhs); }
    /// Point to the object.
    _Ty* operator -> () const { assert(_ptr); return _ptr; }
    /// Dereference the object.
    _Ty& operator * () const { assert(_ptr); return _ptr; }
    /// Convert to the object.
    operator _Ty* () const { return _ptr; }
    
    /// Return the object.
    _Ty* Get() const { return _ptr; }
    /// Return the number of strong references.
    unsigned Refs() const { return _ptr ? _ptr->Refs() : 0; }
    /// Return the number of weak references.
    unsigned WeakRefs() const { return _ptr ? _ptr->WeakRefs() : 0; }
    /// Return whether is a null pointer.
    bool IsNull() const { return _ptr == nullptr; }
    
private:
    /// %AObject pointer.
    _Ty* _ptr;
};

/// Perform a static cast between shared pointers of two types.
template <typename _Ty1, typename _Ty2> SharedPtr<_Ty1> StaticCast(const SharedPtr<_Ty2>& rhs)
{
    SharedPtr<_Ty1> ret;
    ret.StaticCast(rhs);
    return ret;
}

/// Perform a dynamic cast between shared pointers of two types.
template <typename _Ty1, typename _Ty2> SharedPtr<_Ty1> DynamicCast(const SharedPtr<_Ty2>& rhs)
{
    SharedPtr<_Ty1> ret;
    ret.DynamicCast(rhs);
    return ret;
}

/// Pointer which holds a weak reference to a FRefCounted subclass. Can track destruction but does not keep the object alive.
template <typename _Ty> class WeakPtr
{
public:
    /// Construct a null pointer.
    WeakPtr() :
        ptr(nullptr),
        refCount(nullptr)
    {
    }

    /// Copy-construct.
    WeakPtr(const WeakPtr<_Ty>& ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_;
    }

    /// Construct from a shared pointer.
    WeakPtr(const SharedPtr<_Ty>& ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_;
    }

    /// Construct from a raw pointer.
    WeakPtr(_Ty* ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_;
    }

    /// Destruct. Release the object reference.
    ~WeakPtr()
    {
        Reset();
    }

    /// Assign another weak pointer.
    WeakPtr<_Ty>& operator = (const WeakPtr<_Ty>& rhs)
    {
        if (*this == rhs)
            return *this;

        Reset();
        ptr = rhs.ptr;
        refCount = rhs.refCount;
        if (refCount)
            ++(refCount->_weakRefs);
        return *this;
    }

    /// Assign a shared pointer.
    WeakPtr<_Ty>& operator = (const SharedPtr<_Ty>& rhs)
    {
        if (*this == rhs)
            return *this;

        Reset();
        ptr = rhs.Get();
        refCount = ptr ? ptr->RefCountPtr() : nullptr;
        if (refCount)
            ++(refCount->_weakRefs);
        return *this;
    }

    /// Assign a raw pointer.
    WeakPtr<_Ty>& operator = (_Ty* rhs)
    {
        if (Get() == rhs)
            return *this;

        Reset();
        ptr = rhs;
        refCount = ptr ? ptr->RefCountPtr() : nullptr;
        if (refCount)
            ++(refCount->_weakRefs);
        return *this;
    }

    /// Release the weak object reference and reset to null.
    void Reset()
    {
        if (refCount)
        {
            --(refCount->_weakRefs);
            // If expired and no more weak references, destroy the reference count
            if (refCount->_expired && refCount->_weakRefs == 0)
                delete refCount;
            ptr = nullptr;
            refCount = nullptr;
        }
    }

    /// Perform a static cast from a weak pointer of another type.
    template <typename _Oth> void StaticCast(const WeakPtr<_Oth>& rhs)
    {
        *this = static_cast<_Ty*>(rhs.ptr);
    }

    /// Perform a dynamic cast from a weak pointer of another type.
    template <typename _Oth> void DynamicCast(const WeakPtr<_Oth>& rhs)
    {
        Reset();
        _Ty* rhsObject = dynamic_cast<_Ty*>(rhs.ptr);
        if (rhsObject)
            *this = rhsObject;
    }

    /// Test for equality with another weak pointer.
    bool operator == (const WeakPtr<_Ty>& rhs) const { return ptr == rhs.ptr && refCount == rhs.refCount; }
    /// Test for equality with a shared pointer.
    bool operator == (const SharedPtr<_Ty>& rhs) const { return ptr == rhs.Get(); }
    /// Test for equality with a raw pointer.
    bool operator == (_Ty* rhs) const { return ptr == rhs; }
    /// Test for inequality with another weak pointer.
    bool operator != (const WeakPtr<_Ty>& rhs) const { return !(*this == rhs); }
    /// Test for inequality with a shared pointer.
    bool operator != (const SharedPtr<_Ty>& rhs) const { return !(*this == rhs); }
    /// Test for inequality with a raw pointer.
    bool operator != (_Ty* rhs) const { return !(*this == rhs); }
    /// Point to the object.
    _Ty* operator -> () const { _Ty* ret = Get(); assert(ret); return ret; }
    /// Dereference the object.
    _Ty& operator * () const { _Ty* ret = Get(); assert(ret); return ret; }
    /// Convert to the object.
    operator _Ty* () const { return Get(); }

    /// Return the object or null if it has been destroyed.
    _Ty* Get() const
    {
        if (refCount && !refCount->_expired)
            return ptr;
        else
            return nullptr;
    }

    /// Return the number of strong references.
    unsigned Refs() const { return refCount ? refCount->_refs : 0; }
    /// Return the number of weak references.
    unsigned WeakRefs() const { return refCount ? refCount->_weakRefs : 0; }
    /// Return whether is a null pointer.
    bool IsNull() const { return ptr == nullptr; }
    /// Return whether the object has been destroyed. Returns false if is a null pointer.
    bool IsExpired() const { return refCount && refCount->_expired; }

private:
    /// %AObject pointer.
    _Ty* ptr;
    /// The object's weak reference count structure.
    FRefCount* refCount;
};

/// Perform a static cast between weak pointers of two types.
template <typename _Ty, typename _Oth> WeakPtr<_Ty> StaticCast(const WeakPtr<_Oth>& rhs)
{
    WeakPtr<_Ty> ret;
    ret.StaticCast(rhs);
    return ret;
}

/// Perform a dynamic cast between weak pointers of two types.
template <typename _Ty, typename _Oth> WeakPtr<_Ty> DynamicCast(const WeakPtr<_Oth>& rhs)
{
    WeakPtr<_Ty> ret;
    ret.DynamicCast(rhs);
    return ret;
}

/// Pointer which holds a strong reference to an array and allows shared ownership. Uses non-intrusive reference counting.
template <typename _Ty> class SharedArrayPtr
{
public:
    /// Construct a null pointer.
    SharedArrayPtr() :
        _ptr(nullptr),
        _refCount(nullptr)
    {
    }
    
    /// Copy-construct from another shared array pointer.
    SharedArrayPtr(const SharedArrayPtr<_Ty>& ptr_) :
        _ptr(nullptr),
        _refCount(nullptr)
    {
        *this = ptr_;
    }
    
    /// Construct from a raw pointer. To avoid double refcount and double delete, create only once from the same raw pointer.
    explicit SharedArrayPtr(_Ty* ptr_) :
        _ptr(nullptr),
        _refCount(nullptr)
    {
        *this = ptr_;
    }
    
    /// Destruct. Release the array reference.
    ~SharedArrayPtr()
    {
        Reset();
    }
    
    /// Assign from another shared array pointer.
    SharedArrayPtr<_Ty>& operator = (const SharedArrayPtr<_Ty>& rhs)
    {
        if (*this == rhs)
            return *this;
        
        Reset();
        _ptr = rhs._ptr;
        _refCount = rhs._refCount;
        if (_refCount)
            ++(_refCount->_refs);

        return *this;
    }
    
    /// Assign from a raw pointer. To avoid double refcount and double delete, assign only once from the same raw pointer.
    SharedArrayPtr<_Ty>& operator = (_Ty* rhs)
    {
        if (Get() == rhs)
            return *this;
        
        Reset();

        if (rhs)
        {
            _ptr = rhs;
            _refCount = new FRefCount();
            if (_refCount)
                ++(_refCount->_refs);
        }
        
        return *this;
    }
    
    /// Point to the array.
    _Ty* operator -> () const { assert(_ptr); return _ptr; }
    /// Dereference the array.
    _Ty& operator * () const { assert(_ptr); return *_ptr; }
    /// Index the array.
    _Ty& operator [] (size_t index) { assert(_ptr); return _ptr[index]; }
    /// Const-index the array.
    const _Ty& operator [] (size_t index) const { assert(_ptr); return _ptr[index]; }
    /// Test for equality with another shared array pointer.
    bool operator == (const SharedArrayPtr<_Ty>& rhs) const { return _ptr == rhs._ptr; }
    /// Test for inequality with another shared array pointer.
    bool operator != (const SharedArrayPtr<_Ty>& rhs) const { return !(*this == rhs); }
    /// Convert to bool.
    operator bool() const { return _ptr != nullptr; }

    /// Release the array reference and reset to null. Destroy the array if was the last reference.
    void Reset()
    {
        if (_refCount)
        {
            --(_refCount->_refs);
            if (_refCount->_refs == 0)
            {
                _refCount->_expired = true;
                delete[] _ptr;
                // If no weak refs, destroy the ref count now too
                if (_refCount->_weakRefs == 0)
                    delete _refCount;
            }
        }

        _ptr = 0;
        _refCount = 0;
    }
    
    /// Perform a static cast from a shared array pointer of another type.
    template <typename _Oth> void StaticCast(const SharedArrayPtr<_Oth>& rhs)
    {
        Reset();
        _ptr = static_cast<_Ty*>(rhs.Get());
        _refCount = rhs.RefCountPtr();
        if (_refCount)
            ++(_refCount->_refs);
    }
    
   /// Perform a reinterpret cast from a shared array pointer of another type.
    template <typename _Oth> void ReinterpretCast(const SharedArrayPtr<_Oth>& rhs)
    {
        Reset();
        _ptr = reinterpret_cast<_Ty*>(rhs.Get());
        _refCount = rhs.RefCountPtr();
        if (_refCount)
            ++(_refCount->_refs);
    }

    /// Return the raw pointer.
    _Ty* Get() const { return _ptr; }
    /// Return the number of strong references.
    unsigned Refs() const { return _refCount ? _refCount->_refs : 0; }
    /// Return the number of weak references.
    unsigned WeakRefs() const { return _refCount ? _refCount->_weakRefs : 0; }
    /// Return pointer to the reference count structure.
    FRefCount* RefCountPtr() const { return _refCount; }
    /// Check if the pointer is null.
    bool IsNull() const { return _ptr == nullptr; }

private:
    /// Prevent direct assignment from an array pointer of different type.
    template <typename _Oth> SharedArrayPtr<_Ty>& operator = (const SharedArrayPtr<_Oth>& rhs);
    
    /// Pointer to the array.
    _Ty* _ptr;
    /// Pointer to the reference count structure.
    FRefCount* _refCount;
};

/// Perform a static cast from one shared array pointer type to another.
template <typename _Ty, typename _Oth> SharedArrayPtr<_Ty> StaticCast(const SharedArrayPtr<_Oth>& ptr)
{
    SharedArrayPtr<_Ty> ret;
    ret.StaticCast(ptr);
    return ret;
}

/// Perform a reinterpret cast from one shared array pointer type to another.
template <typename _Ty, typename _Oth> SharedArrayPtr<_Ty> ReinterpretCast(const SharedArrayPtr<_Oth>& ptr)
{
    SharedArrayPtr<_Ty> ret;
    ret.ReinterpretCast(ptr);
    return ret;
}

/// Pointer which holds a weak reference to an array. Can track destruction but does not keep the object alive. Uses non-intrusive reference counting.
template <typename _Ty> class WeakArrayPtr
{
public:
    /// Construct a null pointer.
    WeakArrayPtr() :
        _ptr(nullptr),
        _refCount(nullptr)
    {
    }
    
    /// Copy-construct from another weak array pointer.
    WeakArrayPtr(const WeakArrayPtr<_Ty>& ptr) :
        _ptr(nullptr),
        _refCount(nullptr)
    {
        *this = ptr;
    }
    
    /// Construct from a shared array pointer.
    WeakArrayPtr(const SharedArrayPtr<_Ty>& ptr) :
        _ptr(nullptr),
        _refCount(nullptr)
    {
        *this = ptr;
    }
    
    /// Destruct. Release the weak array reference.
    ~WeakArrayPtr()
    {
        Reset();
    }
    
    /// Assign from a shared array pointer.
    WeakArrayPtr<_Ty>& operator = (const SharedArrayPtr<_Ty>& rhs)
    {
        if (_ptr == rhs.Get() && _refCount == rhs.RefCountPtr())
            return *this;
        
        Reset();
        _ptr = rhs.Get();
        _refCount = rhs.RefCountPtr();
        if (_refCount)
            ++(_refCount->_weakRefs);
        
        return *this;
    }
    
    /// Assign from another weak array pointer.
    WeakArrayPtr<_Ty>& operator = (const WeakArrayPtr<_Ty>& rhs)
    {
        if (_ptr == rhs._ptr && _refCount == rhs._refCount)
            return *this;
        
        Reset();
        _ptr = rhs._ptr;
        _refCount = rhs._refCount;
        if (_refCount)
            ++(_refCount->_weakRefs);
        
        return *this;
    }

    /// Point to the array.
    _Ty* operator -> () const
    {
        _Ty* rawPtr = Get();
        assert(rawPtr);
        return rawPtr;
    }
    
    /// Dereference the array.
    _Ty& operator * () const
    {
        _Ty* rawPtr = Get();
        assert(rawPtr);
        return *rawPtr;
    }
    
    /// Index the array.
    _Ty& operator [] (size_t index)
    {
        _Ty* rawPtr = Get();
        assert(rawPtr);
        return (*rawPtr)[index];
    }

    /// Const-index the array.
    const _Ty& operator [] (size_t index) const
    {
        _Ty* rawPtr = Get();
        assert(rawPtr);
        return (*rawPtr)[index];
    }
    
    /// Test for equality with another weak array pointer.
    bool operator == (const WeakArrayPtr<_Ty>& rhs) const { return _ptr == rhs._ptr && _refCount == rhs._refCount; }
    /// Test for equality with a shared array pointer.
    bool operator == (const SharedArrayPtr<_Ty>& rhs) const { return _ptr == rhs._ptr && _refCount == rhs._refCount; }
    /// Test for inequality with another weak array pointer.
    bool operator != (const WeakArrayPtr<_Ty>& rhs) const { return !(*this == rhs); }
    /// Test for inequality with a shared array pointer.
    bool operator != (const SharedArrayPtr<_Ty>& rhs) const { return !(*this == rhs); }
    /// Convert to bool.
    operator bool() const { return Get() != nullptr; }

    /// Release the weak array reference and reset to null.
    void Reset()
    {
        if (_refCount)
        {
            --(_refCount->_weakRefs);
            if (_refCount->_expired && _refCount->_weakRefs == 0)
                delete _refCount;
        }
        
        _ptr = nullptr;
        _refCount = nullptr;
    }
    
    /// Perform a static cast from a weak array pointer of another type.
    template <typename _Oth> void StaticCast(const WeakArrayPtr<_Oth>& rhs)
    {
        Reset();
        _ptr = static_cast<_Ty*>(rhs.Get());
        _refCount = rhs._refCount;
        if (_refCount)
            ++(_refCount->_weakRefs);
    }
    
    /// Perform a reinterpret cast from a weak array pointer of another type.
    template <typename _Oth> void ReinterpretCast(const WeakArrayPtr<_Oth>& rhs)
    {
        Reset();
        _ptr = reinterpret_cast<_Ty*>(rhs.Get());
        _refCount = rhs._refCount;
        if (_refCount)
            ++(_refCount->_weakRefs);
    }
    
    /// Return raw pointer. If array has destroyed, return null.
    _Ty* Get() const
    {
        if (!_refCount || _refCount->_expired)
            return nullptr;
        else
            return _ptr;
    }

    /// Check if the pointer is null.
    bool IsNull() const { return _refCount == nullptr; }
    /// Return number of strong references.
    unsigned Refs() const { return (_refCount && _refCount->_refs >= 0) ? _refCount->_refs : 0; }
    /// Return number of weak references.
    unsigned WeakRefs() const { return _refCount ? _refCount->_weakRefs : 0; }
    /// Return whether the array has been destroyed. Returns false if is a null pointer.
    bool IsExpired() const { return _refCount ? _refCount->_expired : false; }

private:
    /// Prevent direct assignment from a weak array pointer of different type.
    template <typename _Oth> WeakArrayPtr<_Ty>& operator = (const WeakArrayPtr<_Oth>& rhs);

    /// Pointer to the array.
    _Ty* _ptr;
    /// Pointer to the FRefCount structure.
    FRefCount* _refCount;
};

/// Perform a static cast from one weak array pointer type to another.
template <typename _Ty, typename _Oth> WeakArrayPtr<_Ty> StaticCast(const WeakArrayPtr<_Oth>& ptr)
{
    WeakArrayPtr<_Ty> ret;
    ret.StaticCast(ptr);
    return ret;
}

/// Perform a reinterpret cast from one weak pointer type to another.
template <typename _Ty, typename _Oth> WeakArrayPtr<_Ty> ReinterpretCast(const WeakArrayPtr<_Oth>& ptr)
{
    WeakArrayPtr<_Ty> ret;
    ret.ReinterpretCast(ptr);
    return ret;
}

/// Delete object of type T. T must be complete. See boost::checked_delete.
template<typename _Ty> inline void CheckedDelete(_Ty* x)
{
	// intentionally complex - simplification causes regressions
	using type_must_be_complete = char[sizeof(_Ty) ? 1 : -1];
	(void) sizeof(type_must_be_complete);
	delete x;
}

/// Unique pointer template class.
template <typename _Ty> class UniquePtr
{
public:
	/// Construct empty.
	UniquePtr() : _ptr(nullptr) { }

	/// Construct from pointer.
	explicit UniquePtr(_Ty* ptr) : _ptr(ptr) { }

	/// Prevent copy construction.
	UniquePtr(const UniquePtr&) = delete;
	/// Prevent assignment.
	UniquePtr& operator=(const UniquePtr&) = delete;

	/// Assign from pointer.
	UniquePtr& operator = (_Ty* ptr)
	{
		Reset(ptr);
		return *this;
	}

	/// Construct empty.
	UniquePtr(std::nullptr_t) { }   // NOLINT(google-explicit-constructor)

	/// Move-construct from UniquePtr.
	UniquePtr(UniquePtr&& up) noexcept :
		_ptr(up.Detach()) {}

	/// Move-assign from UniquePtr.
	UniquePtr& operator =(UniquePtr&& up) noexcept
	{
		Reset(up.Detach());
		return *this;
	}

	/// Point to the object.
	_Ty* operator ->() const
	{
		assert(_ptr);
		return _ptr;
	}

	/// Dereference the object.
	_Ty& operator *() const
	{
		assert(_ptr);
		return *_ptr;
	}

	/// Test for less than with another unique pointer.
	template <typename _Oth>
	bool operator <(const UniquePtr<_Oth>& rhs) const { return _ptr < rhs._ptr; }

	/// Test for equality with another unique pointer.
	template <typename _Oth>
	bool operator ==(const UniquePtr<_Oth>& rhs) const { return _ptr == rhs._ptr; }

	/// Test for inequality with another unique pointer.
	template <typename _Oth>
	bool operator !=(const UniquePtr<_Oth>& rhs) const { return _ptr != rhs._ptr; }

	/// Cast pointer to bool.
	operator bool() const { return !!_ptr; }    // NOLINT(google-explicit-constructor)

	/// Swap with another UniquePtr.
	void Swap(UniquePtr& up) { Auto3D::Swap(_ptr, up._ptr); }

	/// Detach pointer from UniquePtr without destroying.
	_Ty* Detach()
	{
		_Ty* ptr = _ptr;
		_ptr = nullptr;
		return ptr;
	}

	/// Check if the pointer is null.
	bool Null() const { return _ptr == 0; }

	/// Check if the pointer is not null.
	bool NotNull() const { return _ptr != 0; }

	/// Return the raw pointer.
	_Ty* Get() const { return _ptr; }

	/// Reset.
	void Reset(_Ty* ptr = nullptr)
	{
		CheckedDelete(_ptr);
		_ptr = ptr;
	}

	/// Return hash value for HashSet & HashMap.
	unsigned ToHash() const { return (unsigned)((size_t)_ptr / sizeof(_Ty)); }

	/// Destruct.
	~UniquePtr()
	{
		Reset();
	}

private:
	_Ty* _ptr;

};

/// Swap two UniquePtr-s.
template <typename _Ty> void Swap(UniquePtr<_Ty>& first, UniquePtr<_Ty>& second)
{
	first.Swap(second);
}

/// Construct UniquePtr.
template <typename _Ty, typename ... _Args> UniquePtr<_Ty> MakeUnique(_Args&& ... args)
{
	return UniquePtr<_Ty>(new _Ty(std::forward<_Args>(args)...));
}

/// Construct SharedPtr.
template <typename _Ty, typename ... _Args> SharedPtr<_Ty> MakeShared(_Args&& ... args)
{
	return SharedPtr<_Ty>(new T(std::forward<_Args>(args)...));
}

}
