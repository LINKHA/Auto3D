#pragma once

#include "../AutoConfig.h"

#include <cstddef>

namespace Auto3D
{

/// Random access iterator.
template <typename _Ty> struct RandomAccessIterator
{
    /// Construct a null iterator.
    RandomAccessIterator() :
        _ptr(nullptr)
    {
    }

    /// Construct from a raw pointer.
    explicit RandomAccessIterator(_Ty* ptr_) :
        _ptr(ptr_)
    {
    }

    /// Point to the object.
    _Ty* operator -> () const { return _ptr; }
    /// Dereference the object.
    _Ty& operator * () const { return *_ptr; }
    /// Preincrement the pointer.
    RandomAccessIterator<_Ty>& operator ++ () { ++_ptr; return *this; }
    /// Postincrement the pointer.
    RandomAccessIterator<_Ty> operator ++ (int) { RandomAccessIterator<_Ty> i = *this; ++_ptr; return i; }
    /// Predecrement the pointer.
    RandomAccessIterator<_Ty>& operator -- () { --_ptr; return *this; }
    /// Postdecrement the pointer.
    RandomAccessIterator<_Ty> operator -- (int) { RandomAccessIterator<_Ty> i = *this; --_ptr; return i; }
    /// Add an offset to the pointer.
    RandomAccessIterator<_Ty>& operator += (int value) { _ptr += value; return *this; }
    /// Subtract an offset from the pointer.
    RandomAccessIterator<_Ty>& operator -= (int value) { _ptr -= value; return *this; }
    /// Add an offset to the pointer.
    RandomAccessIterator<_Ty>& operator += (size_t value) { _ptr += value; return *this; }
    /// Subtract an offset from the pointer.
    RandomAccessIterator<_Ty>& operator -= (size_t value) { _ptr -= value; return *this; }
    /// Add an offset to the pointer.
    RandomAccessIterator<_Ty> operator + (int value) const { return RandomAccessIterator<_Ty>(_ptr + value); }
    /// Subtract an offset from the pointer.
    RandomAccessIterator<_Ty> operator - (int value) const { return RandomAccessIterator<_Ty>(_ptr - value); }
    /// Add an offset to the pointer.
    RandomAccessIterator<_Ty> operator + (size_t value) const { return RandomAccessIterator<_Ty>(_ptr + value); }
    /// Subtract an offset from the pointer.
    RandomAccessIterator<_Ty> operator - (size_t value) const { return RandomAccessIterator<_Ty>(_ptr - value); }
    /// Calculate offset to another iterator.
    int operator - (const RandomAccessIterator& rhs) const { return (int)(_ptr - rhs._ptr); }
    /// Test for equality with another iterator.
    bool operator == (const RandomAccessIterator& rhs) const { return _ptr == rhs._ptr; }
    /// Test for inequality with another iterator.
    bool operator != (const RandomAccessIterator& rhs) const { return _ptr != rhs._ptr; }
    /// Test for less than with another iterator.
    bool operator < (const RandomAccessIterator& rhs) const { return _ptr < rhs._ptr; }
    /// Test for greater than with another iterator.
    bool operator > (const RandomAccessIterator& rhs) const { return _ptr > rhs._ptr; }
    /// Test for less than or equal with another iterator.
    bool operator <= (const RandomAccessIterator& rhs) const { return _ptr <= rhs._ptr; }
    /// Test for greater than or equal with another iterator.
    bool operator >= (const RandomAccessIterator& rhs) const { return _ptr >= rhs._ptr; }

    /// Pointer to the random-accessed object(s).
    _Ty* _ptr;
};

/// Random access const iterator.
template <typename _Ty> struct RandomAccessConstIterator
{
    /// Construct a null iterator.
    RandomAccessConstIterator() :
        _ptr(nullptr)
    {
    }

    /// Construct from a raw pointer.
    explicit RandomAccessConstIterator(_Ty* ptr_) :
        _ptr(ptr_)
    {
    }

    /// Construct from a non-const iterator.
    RandomAccessConstIterator(const RandomAccessIterator<_Ty>& it) :
        _ptr(it._ptr)
    {
    }

    /// Assign from a non-const iterator.
    RandomAccessConstIterator<_Ty>& operator = (const RandomAccessIterator<_Ty>& rhs) { _ptr = rhs._ptr; return *this; }
    /// Point to the object.
    const _Ty* operator -> () const { return _ptr; }
    /// Dereference the object.
    const _Ty& operator * () const { return *_ptr; }
    /// Preincrement the pointer.
    RandomAccessConstIterator<_Ty>& operator ++ () { ++_ptr; return *this; }
    /// Postincrement the pointer.
    RandomAccessConstIterator<_Ty> operator ++ (int) { RandomAccessConstIterator<_Ty> i = *this; ++_ptr; return i; }
    /// Predecrement the pointer.
    RandomAccessConstIterator<_Ty>& operator -- () { --_ptr; return *this; }
    /// Postdecrement the pointer.
    RandomAccessConstIterator<_Ty> operator -- (int) { RandomAccessConstIterator<_Ty> i = *this; --_ptr; return i; }
    /// Add an offset to the pointer.
    RandomAccessConstIterator<_Ty>& operator += (int value) { _ptr += value; return *this; }
    /// Subtract an offset from the pointer.
    RandomAccessConstIterator<_Ty>& operator -= (int value) { _ptr -= value; return *this; }
    /// Add an offset to the pointer.
    RandomAccessConstIterator<_Ty>& operator += (size_t value) { _ptr += value; return *this; }
    /// Subtract an offset from the pointer.
    RandomAccessConstIterator<_Ty>& operator -= (size_t value) { _ptr -= value; return *this; }
    /// Add an offset to the pointer.
    RandomAccessConstIterator<_Ty> operator + (int value) const { return RandomAccessConstIterator<_Ty>(_ptr + value); }
    /// Subtract an offset from the pointer.
    RandomAccessConstIterator<_Ty> operator - (int value) const { return RandomAccessConstIterator<_Ty>(_ptr - value); }
    /// Add an offset to the pointer.
    RandomAccessConstIterator<_Ty> operator + (size_t value) const { return RandomAccessConstIterator<_Ty>(_ptr + value); }
    /// Subtract an offset from the pointer.
    RandomAccessConstIterator<_Ty> operator - (size_t value) const { return RandomAccessConstIterator<_Ty>(_ptr - value); }
    /// Calculate offset to another iterator.
    int operator - (const RandomAccessConstIterator& rhs) const { return (int)(_ptr - rhs._ptr); }
    /// Test for equality with another iterator.
    bool operator == (const RandomAccessConstIterator& rhs) const { return _ptr == rhs._ptr; }
    /// Test for inequality with another iterator.
    bool operator != (const RandomAccessConstIterator& rhs) const { return _ptr != rhs._ptr; }
    /// Test for less than with another iterator.
    bool operator < (const RandomAccessConstIterator& rhs) const { return _ptr < rhs._ptr; }
    /// Test for greater than with another iterator.
    bool operator > (const RandomAccessConstIterator& rhs) const { return _ptr > rhs._ptr; }
    /// Test for less than or equal with another iterator.
    bool operator <= (const RandomAccessConstIterator& rhs) const { return _ptr <= rhs._ptr; }
    /// Test for greater than or equal with another iterator.
    bool operator >= (const RandomAccessConstIterator& rhs) const { return _ptr >= rhs._ptr; }

    /// Pointer to the random-accessed object(s).
    _Ty* _ptr;
};

}
