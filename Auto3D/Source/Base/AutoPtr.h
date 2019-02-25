#pragma once

#include "../AutoConfig.h"

#include <cassert>
#include <cstddef>

namespace Auto3D
{

/// Pointer which takes ownership of an object and deletes it when the pointer goes out of scope. Ownership can be transferred to another pointer, in which case the source pointer becomes null.
template <class _Ty> class AutoPtr
{
public:
    /// Construct a null pointer.
    AutoPtr() :
        ptr(nullptr)
    {
    }

    /// Copy-construct. Ownership is transferred, making the source pointer null.
    AutoPtr(const AutoPtr<_Ty>& ptr_) :
        ptr(ptr_.ptr)
    {
        // Trick the compiler so that the AutoPtr can be copied to containers; the latest copy stays non-null
        const_cast<AutoPtr<_Ty>&>(ptr_).ptr = nullptr;
    }

    /// Construct with a raw pointer; take ownership of the object.
    AutoPtr(_Ty* ptr_) :
       ptr(ptr_)
    {
    }

    /// Destruct. Delete the object pointed to.
    ~AutoPtr()
    {
        delete ptr;
    }

    /// Assign from a pointer. Existing object is deleted and ownership is transferred from the source pointer, which becomes null.
    AutoPtr<_Ty>& operator = (const AutoPtr<_Ty>& rhs)
    {
        delete ptr;
        ptr = rhs.ptr;
        const_cast<AutoPtr<_Ty>&>(rhs).ptr = nullptr;
        return *this;
    }

    /// Assign a new object. Existing object is deleted.
    AutoPtr<_Ty>& operator = (_Ty* rhs)
    {
        delete ptr;
        ptr = rhs;
        return *this;
    }

    /// Detach the object from the pointer without destroying it and return it. The pointer becomes null.
    _Ty* Detach()
    {
        _Ty* ret = ptr;
        ptr = nullptr;
        return ret;
    }

    /// Reset to null. Destroys the object.
    void Reset()
    {
        *this = nullptr;
    }
    
    /// Point to the object.
    _Ty* operator -> () const { assert(ptr); return ptr; }
    /// Dereference the object.
    _Ty& operator * () const { assert(ptr); return *ptr; }
    /// Convert to the object.
    operator _Ty* () const { return ptr; }

    /// Return the object.
    _Ty* Get() const { return ptr; }
    /// Return whether is a null pointer.
    bool IsNull() const { return ptr == nullptr; }
    
private:
    /// %Object pointer.
    _Ty* ptr;
};

/// Pointer which takes ownership of an array allocated with new[] and deletes it when the pointer goes out of scope.
template <class _Ty> class AutoArrayPtr
{
public:
    /// Construct a null pointer.
    AutoArrayPtr() :
        array(nullptr)
    {
    }

    /// Copy-construct. Ownership is transferred, making the source pointer null.
    AutoArrayPtr(AutoArrayPtr<_Ty>& ptr) :
        array(ptr.array)
    {
        ptr.array = nullptr;
    }
    
    /// Construct and take ownership of the array.
    AutoArrayPtr(_Ty* array_) :
       array(array_)
    {
    }

    /// Destruct. Delete the array pointed to.
    ~AutoArrayPtr()
    {
        delete[] array;
    }

    /// Assign from a pointer. Existing array is deleted and ownership is transferred from the source pointer, which becomes null.
    AutoArrayPtr<_Ty>& operator = (AutoArrayPtr<_Ty>& rhs)
    {
        delete array;
        array = rhs.array;
        rhs.array = nullptr;
        return *this;
    }

    /// Assign a new array. Existing array is deleted.
    AutoArrayPtr<_Ty>& operator = (_Ty* rhs)
    {
        delete array;
        array = rhs;
        return *this;
    }

    /// Detach the array from the pointer without destroying it and return it. The pointer becomes null.
    _Ty* Detach()
    {
        _Ty* ret = array;
        array = nullptr;
        return ret;
    }

    /// Reset to null. Destroys the array.
    void Reset()
    {
        *this = nullptr;
    }

    /// Point to the array.
    _Ty* operator -> () const { assert(array); return array; }
    /// Dereference the array.
    _Ty& operator * () const { assert(array); return *array; }
    /// Index the array.
    _Ty& operator [] (size_t index) { assert(array); return array[index]; }
    /// Const-index the array.
    const _Ty& operator [] (size_t index) const { assert(array); return array[index]; }
    /// Convert to bool.
    operator bool () const { return array != nullptr; }

    /// Return the array.
    _Ty* Get() const { return array; }
    /// Return whether is a null pointer.
    bool IsNull() const { return array == nullptr; }
    
private:
    /// Array pointer.
    _Ty* array;
};

}
