#pragma once

#include "AutoConfig.h"

#include <cstddef>

namespace Auto3D
{

/// Random access iterator.
template <typename _Ty> struct TRandomAccessIterator
{
    /// Construct a null iterator.
    TRandomAccessIterator() :
        _ptr(nullptr)
    {
    }

    /// Construct from a raw pointer.
    explicit TRandomAccessIterator(_Ty* ptr_) :
        _ptr(ptr_)
    {
    }

    /// Point to the object.
    _Ty* operator -> () const { return _ptr; }
    /// Dereference the object.
    _Ty& operator * () const { return *_ptr; }
    /// Preincrement the pointer.
    TRandomAccessIterator<_Ty>& operator ++ () { ++_ptr; return *this; }
    /// Postincrement the pointer.
    TRandomAccessIterator<_Ty> operator ++ (int) { TRandomAccessIterator<_Ty> i = *this; ++_ptr; return i; }
    /// Predecrement the pointer.
    TRandomAccessIterator<_Ty>& operator -- () { --_ptr; return *this; }
    /// Postdecrement the pointer.
    TRandomAccessIterator<_Ty> operator -- (int) { TRandomAccessIterator<_Ty> i = *this; --_ptr; return i; }
    /// Add an offset to the pointer.
    TRandomAccessIterator<_Ty>& operator += (int value) { _ptr += value; return *this; }
    /// Subtract an offset from the pointer.
    TRandomAccessIterator<_Ty>& operator -= (int value) { _ptr -= value; return *this; }
    /// Add an offset to the pointer.
    TRandomAccessIterator<_Ty>& operator += (size_t value) { _ptr += value; return *this; }
    /// Subtract an offset from the pointer.
    TRandomAccessIterator<_Ty>& operator -= (size_t value) { _ptr -= value; return *this; }
    /// Add an offset to the pointer.
    TRandomAccessIterator<_Ty> operator + (int value) const { return TRandomAccessIterator<_Ty>(_ptr + value); }
    /// Subtract an offset from the pointer.
    TRandomAccessIterator<_Ty> operator - (int value) const { return TRandomAccessIterator<_Ty>(_ptr - value); }
    /// Add an offset to the pointer.
    TRandomAccessIterator<_Ty> operator + (size_t value) const { return TRandomAccessIterator<_Ty>(_ptr + value); }
    /// Subtract an offset from the pointer.
    TRandomAccessIterator<_Ty> operator - (size_t value) const { return TRandomAccessIterator<_Ty>(_ptr - value); }
    /// Calculate offset to another iterator.
    int operator - (const TRandomAccessIterator& rhs) const { return (int)(_ptr - rhs._ptr); }
    /// Test for equality with another iterator.
    bool operator == (const TRandomAccessIterator& rhs) const { return _ptr == rhs._ptr; }
    /// Test for inequality with another iterator.
    bool operator != (const TRandomAccessIterator& rhs) const { return _ptr != rhs._ptr; }
    /// Test for less than with another iterator.
    bool operator < (const TRandomAccessIterator& rhs) const { return _ptr < rhs._ptr; }
    /// Test for greater than with another iterator.
    bool operator > (const TRandomAccessIterator& rhs) const { return _ptr > rhs._ptr; }
    /// Test for less than or equal with another iterator.
    bool operator <= (const TRandomAccessIterator& rhs) const { return _ptr <= rhs._ptr; }
    /// Test for greater than or equal with another iterator.
    bool operator >= (const TRandomAccessIterator& rhs) const { return _ptr >= rhs._ptr; }

    /// Pointer to the random-accessed object(s).
    _Ty* _ptr;
};

/// Random access const iterator.
template <typename _Ty> struct TRandomAccessConstIterator
{
    /// Construct a null iterator.
    TRandomAccessConstIterator() :
        _ptr(nullptr)
    {
    }

    /// Construct from a raw pointer.
    explicit TRandomAccessConstIterator(_Ty* ptr_) :
        _ptr(ptr_)
    {
    }

    /// Construct from a non-const iterator.
    TRandomAccessConstIterator(const TRandomAccessIterator<_Ty>& it) :
        _ptr(it._ptr)
    {
    }

    /// Assign from a non-const iterator.
    TRandomAccessConstIterator<_Ty>& operator = (const TRandomAccessIterator<_Ty>& rhs) { _ptr = rhs._ptr; return *this; }
    /// Point to the object.
    const _Ty* operator -> () const { return _ptr; }
    /// Dereference the object.
    const _Ty& operator * () const { return *_ptr; }
    /// Preincrement the pointer.
    TRandomAccessConstIterator<_Ty>& operator ++ () { ++_ptr; return *this; }
    /// Postincrement the pointer.
    TRandomAccessConstIterator<_Ty> operator ++ (int) { TRandomAccessConstIterator<_Ty> i = *this; ++_ptr; return i; }
    /// Predecrement the pointer.
    TRandomAccessConstIterator<_Ty>& operator -- () { --_ptr; return *this; }
    /// Postdecrement the pointer.
    TRandomAccessConstIterator<_Ty> operator -- (int) { TRandomAccessConstIterator<_Ty> i = *this; --_ptr; return i; }
    /// Add an offset to the pointer.
    TRandomAccessConstIterator<_Ty>& operator += (int value) { _ptr += value; return *this; }
    /// Subtract an offset from the pointer.
    TRandomAccessConstIterator<_Ty>& operator -= (int value) { _ptr -= value; return *this; }
    /// Add an offset to the pointer.
    TRandomAccessConstIterator<_Ty>& operator += (size_t value) { _ptr += value; return *this; }
    /// Subtract an offset from the pointer.
    TRandomAccessConstIterator<_Ty>& operator -= (size_t value) { _ptr -= value; return *this; }
    /// Add an offset to the pointer.
    TRandomAccessConstIterator<_Ty> operator + (int value) const { return TRandomAccessConstIterator<_Ty>(_ptr + value); }
    /// Subtract an offset from the pointer.
    TRandomAccessConstIterator<_Ty> operator - (int value) const { return TRandomAccessConstIterator<_Ty>(_ptr - value); }
    /// Add an offset to the pointer.
    TRandomAccessConstIterator<_Ty> operator + (size_t value) const { return TRandomAccessConstIterator<_Ty>(_ptr + value); }
    /// Subtract an offset from the pointer.
    TRandomAccessConstIterator<_Ty> operator - (size_t value) const { return TRandomAccessConstIterator<_Ty>(_ptr - value); }
    /// Calculate offset to another iterator.
    int operator - (const TRandomAccessConstIterator& rhs) const { return (int)(_ptr - rhs._ptr); }
    /// Test for equality with another iterator.
    bool operator == (const TRandomAccessConstIterator& rhs) const { return _ptr == rhs._ptr; }
    /// Test for inequality with another iterator.
    bool operator != (const TRandomAccessConstIterator& rhs) const { return _ptr != rhs._ptr; }
    /// Test for less than with another iterator.
    bool operator < (const TRandomAccessConstIterator& rhs) const { return _ptr < rhs._ptr; }
    /// Test for greater than with another iterator.
    bool operator > (const TRandomAccessConstIterator& rhs) const { return _ptr > rhs._ptr; }
    /// Test for less than or equal with another iterator.
    bool operator <= (const TRandomAccessConstIterator& rhs) const { return _ptr <= rhs._ptr; }
    /// Test for greater than or equal with another iterator.
    bool operator >= (const TRandomAccessConstIterator& rhs) const { return _ptr >= rhs._ptr; }

    /// Pointer to the random-accessed object(s).
    _Ty* _ptr;
};

}
