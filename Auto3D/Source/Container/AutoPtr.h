#pragma once

#include "AutoConfig.h"

#include <cassert>
#include <cstddef>

namespace Auto3D
{

/// Pointer which takes ownership of an object and deletes it when the pointer goes out of scope. Ownership can be transferred to another pointer, in which case the source pointer becomes null.
template <typename _Ty> class TAutoPtr
{
public:
    /// Construct a null pointer.
    TAutoPtr() :
        _ptr(nullptr)
    {
    }

    /// Copy-construct. Ownership is transferred, making the source pointer null.
    TAutoPtr(const TAutoPtr<_Ty>& ptr) :
        _ptr(ptr._ptr)
    {
        // Trick the compiler so that the TAutoPtr can be copied to containers; the latest copy stays non-null
        const_cast<TAutoPtr<_Ty>&>(ptr)._ptr = nullptr;
    }

    /// Construct with a raw pointer; take ownership of the object.
    TAutoPtr(_Ty* ptr) :
       _ptr(ptr)
    {
    }

    /// Destruct. Delete the object pointed to.
    ~TAutoPtr()
    {
        delete _ptr;
    }

    /// Assign from a pointer. Existing object is deleted and ownership is transferred from the source pointer, which becomes null.
    TAutoPtr<_Ty>& operator = (const TAutoPtr<_Ty>& rhs)
    {
        delete _ptr;
        _ptr = rhs._ptr;
        const_cast<TAutoPtr<_Ty>&>(rhs)._ptr = nullptr;
        return *this;
    }

    /// Assign a new object. Existing object is deleted.
    TAutoPtr<_Ty>& operator = (_Ty* rhs)
    {
        delete _ptr;
        _ptr = rhs;
        return *this;
    }

    /// Detach the object from the pointer without destroying it and return it. The pointer becomes null.
    _Ty* Detach()
    {
        _Ty* ret = _ptr;
        _ptr = nullptr;
        return ret;
    }

    /// Reset to null. Destroys the object.
    void Reset()
    {
        *this = nullptr;
    }
    
    /// Point to the object.
    _Ty* operator -> () const { assert(_ptr); return _ptr; }
    /// Dereference the object.
    _Ty& operator * () const { assert(_ptr); return *_ptr; }
    /// Convert to the object.
    operator _Ty* () const { return _ptr; }

    /// Return the object.
    _Ty* Get() const { return _ptr; }
    /// Return whether is a null pointer.
    bool IsNull() const { return _ptr == nullptr; }
    
private:
    /// %AObject pointer.
    _Ty* _ptr;
};

/// Pointer which takes ownership of an array allocated with new[] and deletes it when the pointer goes out of scope.
template <typename _Ty> class TAutoArrayPtr
{
public:
    /// Construct a null pointer.
    TAutoArrayPtr() :
        _arrayPtrs(nullptr)
    {
    }

    /// Copy-construct. Ownership is transferred, making the source pointer null.
    TAutoArrayPtr(TAutoArrayPtr<_Ty>& ptr) :
        _arrayPtrs(ptr._arrayPtrs)
    {
        ptr._arrayPtrs = nullptr;
    }
    
    /// Construct and take ownership of the array.
    TAutoArrayPtr(_Ty* array_) :
       _arrayPtrs(array_)
    {
    }

    /// Destruct. Delete the array pointed to.
    ~TAutoArrayPtr()
    {
        delete[] _arrayPtrs;
    }

    /// Assign from a pointer. Existing array is deleted and ownership is transferred from the source pointer, which becomes null.
    TAutoArrayPtr<_Ty>& operator = (TAutoArrayPtr<_Ty>& rhs)
    {
        delete _arrayPtrs;
        _arrayPtrs = rhs._arrayPtrs;
        rhs._arrayPtrs = nullptr;
        return *this;
    }

    /// Assign a new array. Existing array is deleted.
    TAutoArrayPtr<_Ty>& operator = (_Ty* rhs)
    {
        delete _arrayPtrs;
        _arrayPtrs = rhs;
        return *this;
    }

    /// Detach the array from the pointer without destroying it and return it. The pointer becomes null.
    _Ty* Detach()
    {
        _Ty* ret = _arrayPtrs;
        _arrayPtrs = nullptr;
        return ret;
    }

    /// Reset to null. Destroys the array.
    void Reset()
    {
        *this = nullptr;
    }

    /// Point to the array.
    _Ty* operator -> () const { assert(_arrayPtrs); return _arrayPtrs; }
    /// Dereference the array.
    _Ty& operator * () const { assert(_arrayPtrs); return *_arrayPtrs; }
    /// Index the array.
    _Ty& operator [] (size_t index) { assert(_arrayPtrs); return _arrayPtrs[index]; }
    /// Const-index the array.
    const _Ty& operator [] (size_t index) const { assert(_arrayPtrs); return _arrayPtrs[index]; }
    /// Convert to bool.
    operator bool () const { return _arrayPtrs != nullptr; }

    /// Return the array.
    _Ty* Get() const { return _arrayPtrs; }
    /// Return whether is a null pointer.
    bool IsNull() const { return _arrayPtrs == nullptr; }
    
private:
    /// TArray pointer.
    _Ty* _arrayPtrs;
};

}
