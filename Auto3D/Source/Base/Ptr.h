#pragma once

#include "../AutoConfig.h"

#include <cassert>
#include <cstddef>

namespace Auto3D
{

class RefCounted;
template <class _Ty> class WeakPtr;

/// Reference count structure. Used in both intrusive and non-intrusive reference counting.
struct AUTO_API RefCount
{
    /// Construct with zero refcounts.
    RefCount() :
        refs(0),
        weakRefs(0),
        expired(false)
    {
    }

    /// Number of strong references. These keep the object alive.
    unsigned refs;
    /// Number of weak references.
    unsigned weakRefs;
    /// Expired flag. The object is no longer safe to access after this is set true.
    bool expired;
};

/// Base class for intrusively reference counted objects that can be pointed to with SharedPtr and WeakPtr. These are not copy-constructible and not assignable.
class AUTO_API RefCounted
{
public:
    /// Construct. The reference count is not allocated yet; it will be allocated on demand.
    RefCounted();

    /// Destruct. If no weak references, destroy also the reference count, else mark it expired.
    virtual ~RefCounted();

    /// Add a strong reference. Allocate the reference count structure first if necessary.
    void AddRef();
    /// Release a strong reference. Destroy the object when the last strong reference is gone.
    void ReleaseRef();

    /// Return the number of strong references.
    unsigned Refs() const { return refCount ? refCount->refs : 0; }
    /// Return the number of weak references.
    unsigned WeakRefs() const { return refCount ? refCount->weakRefs : 0; }
    /// Return pointer to the reference count structure. Allocate if not allocated yet.
    RefCount* RefCountPtr();

private:
    /// Prevent copy construction.
    RefCounted(const RefCounted& rhs);
    /// Prevent assignment.
    RefCounted& operator = (const RefCounted& rhs);

    /// Reference count structure, allocated on demand.
    RefCount* refCount;
};

/// Pointer which holds a strong reference to a RefCounted subclass and allows shared ownership.
template <class _Ty> class SharedPtr
{
public:
    /// Construct a null pointer.
    SharedPtr() :
        ptr(nullptr)
    {
    }
    
    /// Copy-construct.
    SharedPtr(const SharedPtr<_Ty>& ptr_) :
        ptr(nullptr)
    {
        *this = ptr_;
    }

    /// Construct from a raw pointer.
    SharedPtr(_Ty* ptr_) :
        ptr(nullptr)
    {
        *this = ptr_;
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
        ptr = rhs;
        if (ptr)
            ptr->AddRef();
        return *this;
    }
    
    /// Assign another shared pointer.
    SharedPtr<_Ty>& operator = (const SharedPtr<_Ty>& rhs)
    {
        if (*this == rhs)
            return *this;

        Reset();
        ptr = rhs.ptr;
        if (ptr)
            ptr->AddRef();
        return *this;
    }
    
    /// Release the object reference and reset to null. Destroy the object if was the last reference.
    void Reset()
    {
        if (ptr)
        {
            ptr->ReleaseRef();
            ptr = nullptr;
        }
    }
    
    /// Perform a static cast from a shared pointer of another type.
    template <class U> void StaticCast(const SharedPtr<U>& rhs)
    {
        *this = static_cast<_Ty*>(rhs.ptr);
    }

    /// Perform a dynamic cast from a weak pointer of another type.
    template <class U> void DynamicCast(const WeakPtr<U>& rhs)
    {
        Reset();
        _Ty* rhsObject = dynamic_cast<_Ty*>(rhs.ptr);
        if (rhsObject)
            *this = rhsObject;
    }
    
    /// Test for equality with another shared pointer.
    bool operator == (const SharedPtr<_Ty>& rhs) const { return ptr == rhs.ptr; }
    /// Test for equality with a raw pointer.
    bool operator == (_Ty* rhs) const { return ptr == rhs; }
    /// Test for inequality with another shared pointer.
    bool operator != (const SharedPtr<_Ty>& rhs) const { return !(*this == rhs); }
    /// Test for inequality with a raw pointer.
    bool operator != (_Ty* rhs) const { return !(*this == rhs); }
    /// Point to the object.
    _Ty* operator -> () const { assert(ptr); return ptr; }
    /// Dereference the object.
    _Ty& operator * () const { assert(ptr); return ptr; }
    /// Convert to the object.
    operator _Ty* () const { return ptr; }
    
    /// Return the object.
    _Ty* Get() const { return ptr; }
    /// Return the number of strong references.
    unsigned Refs() const { return ptr ? ptr->Refs() : 0; }
    /// Return the number of weak references.
    unsigned WeakRefs() const { return ptr ? ptr->WeakRefs() : 0; }
    /// Return whether is a null pointer.
    bool IsNull() const { return ptr == nullptr; }
    
private:
    /// %Object pointer.
    _Ty* ptr;
};

/// Perform a static cast between shared pointers of two types.
template <class _Ty, class U> SharedPtr<_Ty> StaticCast(const SharedPtr<U>& rhs)
{
    SharedPtr<_Ty> ret;
    ret.StaticCast(rhs);
    return ret;
}

/// Perform a dynamic cast between shared pointers of two types.
template <class _Ty, class U> SharedPtr<_Ty> DynamicCast(const SharedPtr<U>& rhs)
{
    SharedPtr<_Ty> ret;
    ret.DynamicCast(rhs);
    return ret;
}

/// Pointer which holds a weak reference to a RefCounted subclass. Can track destruction but does not keep the object alive.
template <class _Ty> class WeakPtr
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
            ++(refCount->weakRefs);
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
            ++(refCount->weakRefs);
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
            ++(refCount->weakRefs);
        return *this;
    }

    /// Release the weak object reference and reset to null.
    void Reset()
    {
        if (refCount)
        {
            --(refCount->weakRefs);
            // If expired and no more weak references, destroy the reference count
            if (refCount->expired && refCount->weakRefs == 0)
                delete refCount;
            ptr = nullptr;
            refCount = nullptr;
        }
    }

    /// Perform a static cast from a weak pointer of another type.
    template <class U> void StaticCast(const WeakPtr<U>& rhs)
    {
        *this = static_cast<_Ty*>(rhs.ptr);
    }

    /// Perform a dynamic cast from a weak pointer of another type.
    template <class U> void DynamicCast(const WeakPtr<U>& rhs)
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
        if (refCount && !refCount->expired)
            return ptr;
        else
            return nullptr;
    }

    /// Return the number of strong references.
    unsigned Refs() const { return refCount ? refCount->refs : 0; }
    /// Return the number of weak references.
    unsigned WeakRefs() const { return refCount ? refCount->weakRefs : 0; }
    /// Return whether is a null pointer.
    bool IsNull() const { return ptr == nullptr; }
    /// Return whether the object has been destroyed. Returns false if is a null pointer.
    bool IsExpired() const { return refCount && refCount->expired; }

private:
    /// %Object pointer.
    _Ty* ptr;
    /// The object's weak reference count structure.
    RefCount* refCount;
};

/// Perform a static cast between weak pointers of two types.
template <class _Ty, class U> WeakPtr<_Ty> StaticCast(const WeakPtr<U>& rhs)
{
    WeakPtr<_Ty> ret;
    ret.StaticCast(rhs);
    return ret;
}

/// Perform a dynamic cast between weak pointers of two types.
template <class _Ty, class U> WeakPtr<_Ty> DynamicCast(const WeakPtr<U>& rhs)
{
    WeakPtr<_Ty> ret;
    ret.DynamicCast(rhs);
    return ret;
}

/// Pointer which holds a strong reference to an array and allows shared ownership. Uses non-intrusive reference counting.
template <class _Ty> class SharedArrayPtr
{
public:
    /// Construct a null pointer.
    SharedArrayPtr() :
        ptr(nullptr),
        refCount(nullptr)
    {
    }
    
    /// Copy-construct from another shared array pointer.
    SharedArrayPtr(const SharedArrayPtr<_Ty>& ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_;
    }
    
    /// Construct from a raw pointer. To avoid double refcount and double delete, create only once from the same raw pointer.
    explicit SharedArrayPtr(_Ty* ptr_) :
        ptr(nullptr),
        refCount(nullptr)
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
        ptr = rhs.ptr;
        refCount = rhs.refCount;
        if (refCount)
            ++(refCount->refs);

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
            ptr = rhs;
            refCount = new RefCount();
            if (refCount)
                ++(refCount->refs);
        }
        
        return *this;
    }
    
    /// Point to the array.
    _Ty* operator -> () const { assert(ptr); return ptr; }
    /// Dereference the array.
    _Ty& operator * () const { assert(ptr); return *ptr; }
    /// Index the array.
    _Ty& operator [] (size_t index) { assert(ptr); return ptr[index]; }
    /// Const-index the array.
    const _Ty& operator [] (size_t index) const { assert(ptr); return ptr[index]; }
    /// Test for equality with another shared array pointer.
    bool operator == (const SharedArrayPtr<_Ty>& rhs) const { return ptr == rhs.ptr; }
    /// Test for inequality with another shared array pointer.
    bool operator != (const SharedArrayPtr<_Ty>& rhs) const { return !(*this == rhs); }
    /// Convert to bool.
    operator bool() const { return ptr != nullptr; }

    /// Release the array reference and reset to null. Destroy the array if was the last reference.
    void Reset()
    {
        if (refCount)
        {
            --(refCount->refs);
            if (refCount->refs == 0)
            {
                refCount->expired = true;
                delete[] ptr;
                // If no weak refs, destroy the ref count now too
                if (refCount->weakRefs == 0)
                    delete refCount;
            }
        }

        ptr = 0;
        refCount = 0;
    }
    
    /// Perform a static cast from a shared array pointer of another type.
    template <class U> void StaticCast(const SharedArrayPtr<U>& rhs)
    {
        Reset();
        ptr = static_cast<_Ty*>(rhs.Get());
        refCount = rhs.RefCountPtr();
        if (refCount)
            ++(refCount->refs);
    }
    
   /// Perform a reinterpret cast from a shared array pointer of another type.
    template <class U> void ReinterpretCast(const SharedArrayPtr<U>& rhs)
    {
        Reset();
        ptr = reinterpret_cast<_Ty*>(rhs.Get());
        refCount = rhs.RefCountPtr();
        if (refCount)
            ++(refCount->refs);
    }

    /// Return the raw pointer.
    _Ty* Get() const { return ptr; }
    /// Return the number of strong references.
    unsigned Refs() const { return refCount ? refCount->refs : 0; }
    /// Return the number of weak references.
    unsigned WeakRefs() const { return refCount ? refCount->weakRefs : 0; }
    /// Return pointer to the reference count structure.
    RefCount* RefCountPtr() const { return refCount; }
    /// Check if the pointer is null.
    bool IsNull() const { return ptr == nullptr; }

private:
    /// Prevent direct assignment from an array pointer of different type.
    template <class U> SharedArrayPtr<_Ty>& operator = (const SharedArrayPtr<U>& rhs);
    
    /// Pointer to the array.
    _Ty* ptr;
    /// Pointer to the reference count structure.
    RefCount* refCount;
};

/// Perform a static cast from one shared array pointer type to another.
template <class _Ty, class U> SharedArrayPtr<_Ty> StaticCast(const SharedArrayPtr<U>& ptr)
{
    SharedArrayPtr<_Ty> ret;
    ret.StaticCast(ptr);
    return ret;
}

/// Perform a reinterpret cast from one shared array pointer type to another.
template <class _Ty, class U> SharedArrayPtr<_Ty> ReinterpretCast(const SharedArrayPtr<U>& ptr)
{
    SharedArrayPtr<_Ty> ret;
    ret.ReinterpretCast(ptr);
    return ret;
}

/// Pointer which holds a weak reference to an array. Can track destruction but does not keep the object alive. Uses non-intrusive reference counting.
template <class _Ty> class WeakArrayPtr
{
public:
    /// Construct a null pointer.
    WeakArrayPtr() :
        ptr(nullptr),
        refCount(nullptr)
    {
    }
    
    /// Copy-construct from another weak array pointer.
    WeakArrayPtr(const WeakArrayPtr<_Ty>& ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_;
    }
    
    /// Construct from a shared array pointer.
    WeakArrayPtr(const SharedArrayPtr<_Ty>& ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_;
    }
    
    /// Destruct. Release the weak array reference.
    ~WeakArrayPtr()
    {
        Reset();
    }
    
    /// Assign from a shared array pointer.
    WeakArrayPtr<_Ty>& operator = (const SharedArrayPtr<_Ty>& rhs)
    {
        if (ptr == rhs.Get() && refCount == rhs.RefCountPtr())
            return *this;
        
        Reset();
        ptr = rhs.Get();
        refCount = rhs.RefCountPtr();
        if (refCount)
            ++(refCount->weakRefs);
        
        return *this;
    }
    
    /// Assign from another weak array pointer.
    WeakArrayPtr<_Ty>& operator = (const WeakArrayPtr<_Ty>& rhs)
    {
        if (ptr == rhs.ptr && refCount == rhs.refCount)
            return *this;
        
        Reset();
        ptr = rhs.ptr;
        refCount = rhs.refCount;
        if (refCount)
            ++(refCount->weakRefs);
        
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
    bool operator == (const WeakArrayPtr<_Ty>& rhs) const { return ptr == rhs.ptr && refCount == rhs.refCount; }
    /// Test for equality with a shared array pointer.
    bool operator == (const SharedArrayPtr<_Ty>& rhs) const { return ptr == rhs.ptr && refCount == rhs.refCount; }
    /// Test for inequality with another weak array pointer.
    bool operator != (const WeakArrayPtr<_Ty>& rhs) const { return !(*this == rhs); }
    /// Test for inequality with a shared array pointer.
    bool operator != (const SharedArrayPtr<_Ty>& rhs) const { return !(*this == rhs); }
    /// Convert to bool.
    operator bool() const { return Get() != nullptr; }

    /// Release the weak array reference and reset to null.
    void Reset()
    {
        if (refCount)
        {
            --(refCount->weakRefs);
            if (refCount->expired && refCount->weakRefs == 0)
                delete refCount;
        }
        
        ptr = nullptr;
        refCount = nullptr;
    }
    
    /// Perform a static cast from a weak array pointer of another type.
    template <class U> void StaticCast(const WeakArrayPtr<U>& rhs)
    {
        Reset();
        ptr = static_cast<_Ty*>(rhs.Get());
        refCount = rhs.refCount;
        if (refCount)
            ++(refCount->weakRefs);
    }
    
    /// Perform a reinterpret cast from a weak array pointer of another type.
    template <class U> void ReinterpretCast(const WeakArrayPtr<U>& rhs)
    {
        Reset();
        ptr = reinterpret_cast<_Ty*>(rhs.Get());
        refCount = rhs.refCount;
        if (refCount)
            ++(refCount->weakRefs);
    }
    
    /// Return raw pointer. If array has destroyed, return null.
    _Ty* Get() const
    {
        if (!refCount || refCount->expired)
            return nullptr;
        else
            return ptr;
    }

    /// Check if the pointer is null.
    bool IsNull() const { return refCount == nullptr; }
    /// Return number of strong references.
    unsigned Refs() const { return (refCount && refCount->refs >= 0) ? refCount->refs : 0; }
    /// Return number of weak references.
    unsigned WeakRefs() const { return refCount ? refCount->weakRefs : 0; }
    /// Return whether the array has been destroyed. Returns false if is a null pointer.
    bool IsExpired() const { return refCount ? refCount->expired : false; }

private:
    /// Prevent direct assignment from a weak array pointer of different type.
    template <class U> WeakArrayPtr<_Ty>& operator = (const WeakArrayPtr<U>& rhs);

    /// Pointer to the array.
    _Ty* ptr;
    /// Pointer to the RefCount structure.
    RefCount* refCount;
};

/// Perform a static cast from one weak array pointer type to another.
template <class _Ty, class U> WeakArrayPtr<_Ty> StaticCast(const WeakArrayPtr<U>& ptr)
{
    WeakArrayPtr<_Ty> ret;
    ret.StaticCast(ptr);
    return ret;
}

/// Perform a reinterpret cast from one weak pointer type to another.
template <class _Ty, class U> WeakArrayPtr<_Ty> ReinterpretCast(const WeakArrayPtr<U>& ptr)
{
    WeakArrayPtr<_Ty> ret;
    ret.ReinterpretCast(ptr);
    return ret;
}

}
