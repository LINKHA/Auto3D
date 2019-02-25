#pragma once

#include "../AutoConfig.h"

#include <cstddef>

namespace Auto3D
{

/// Random access iterator.
template <class _Ty> struct RandomAccessIterator
{
    /// Construct a null iterator.
    RandomAccessIterator() :
        ptr(nullptr)
    {
    }

    /// Construct from a raw pointer.
    explicit RandomAccessIterator(_Ty* ptr_) :
        ptr(ptr_)
    {
    }

    /// Point to the object.
    _Ty* operator -> () const { return ptr; }
    /// Dereference the object.
    _Ty& operator * () const { return *ptr; }
    /// Preincrement the pointer.
    RandomAccessIterator<_Ty>& operator ++ () { ++ptr; return *this; }
    /// Postincrement the pointer.
    RandomAccessIterator<_Ty> operator ++ (int) { RandomAccessIterator<_Ty> i = *this; ++ptr; return i; }
    /// Predecrement the pointer.
    RandomAccessIterator<_Ty>& operator -- () { --ptr; return *this; }
    /// Postdecrement the pointer.
    RandomAccessIterator<_Ty> operator -- (int) { RandomAccessIterator<_Ty> i = *this; --ptr; return i; }
    /// Add an offset to the pointer.
    RandomAccessIterator<_Ty>& operator += (int value) { ptr += value; return *this; }
    /// Subtract an offset from the pointer.
    RandomAccessIterator<_Ty>& operator -= (int value) { ptr -= value; return *this; }
    /// Add an offset to the pointer.
    RandomAccessIterator<_Ty>& operator += (size_t value) { ptr += value; return *this; }
    /// Subtract an offset from the pointer.
    RandomAccessIterator<_Ty>& operator -= (size_t value) { ptr -= value; return *this; }
    /// Add an offset to the pointer.
    RandomAccessIterator<_Ty> operator + (int value) const { return RandomAccessIterator<_Ty>(ptr + value); }
    /// Subtract an offset from the pointer.
    RandomAccessIterator<_Ty> operator - (int value) const { return RandomAccessIterator<_Ty>(ptr - value); }
    /// Add an offset to the pointer.
    RandomAccessIterator<_Ty> operator + (size_t value) const { return RandomAccessIterator<_Ty>(ptr + value); }
    /// Subtract an offset from the pointer.
    RandomAccessIterator<_Ty> operator - (size_t value) const { return RandomAccessIterator<_Ty>(ptr - value); }
    /// Calculate offset to another iterator.
    int operator - (const RandomAccessIterator& rhs) const { return (int)(ptr - rhs.ptr); }
    /// Test for equality with another iterator.
    bool operator == (const RandomAccessIterator& rhs) const { return ptr == rhs.ptr; }
    /// Test for inequality with another iterator.
    bool operator != (const RandomAccessIterator& rhs) const { return ptr != rhs.ptr; }
    /// Test for less than with another iterator.
    bool operator < (const RandomAccessIterator& rhs) const { return ptr < rhs.ptr; }
    /// Test for greater than with another iterator.
    bool operator > (const RandomAccessIterator& rhs) const { return ptr > rhs.ptr; }
    /// Test for less than or equal with another iterator.
    bool operator <= (const RandomAccessIterator& rhs) const { return ptr <= rhs.ptr; }
    /// Test for greater than or equal with another iterator.
    bool operator >= (const RandomAccessIterator& rhs) const { return ptr >= rhs.ptr; }

    /// Pointer to the random-accessed object(s).
    _Ty* ptr;
};

/// Random access const iterator.
template <class _Ty> struct RandomAccessConstIterator
{
    /// Construct a null iterator.
    RandomAccessConstIterator() :
        ptr(nullptr)
    {
    }

    /// Construct from a raw pointer.
    explicit RandomAccessConstIterator(_Ty* ptr_) :
        ptr(ptr_)
    {
    }

    /// Construct from a non-const iterator.
    RandomAccessConstIterator(const RandomAccessIterator<_Ty>& it) :
        ptr(it.ptr)
    {
    }

    /// Assign from a non-const iterator.
    RandomAccessConstIterator<_Ty>& operator = (const RandomAccessIterator<_Ty>& rhs) { ptr = rhs.ptr; return *this; }
    /// Point to the object.
    const _Ty* operator -> () const { return ptr; }
    /// Dereference the object.
    const _Ty& operator * () const { return *ptr; }
    /// Preincrement the pointer.
    RandomAccessConstIterator<_Ty>& operator ++ () { ++ptr; return *this; }
    /// Postincrement the pointer.
    RandomAccessConstIterator<_Ty> operator ++ (int) { RandomAccessConstIterator<_Ty> i = *this; ++ptr; return i; }
    /// Predecrement the pointer.
    RandomAccessConstIterator<_Ty>& operator -- () { --ptr; return *this; }
    /// Postdecrement the pointer.
    RandomAccessConstIterator<_Ty> operator -- (int) { RandomAccessConstIterator<_Ty> i = *this; --ptr; return i; }
    /// Add an offset to the pointer.
    RandomAccessConstIterator<_Ty>& operator += (int value) { ptr += value; return *this; }
    /// Subtract an offset from the pointer.
    RandomAccessConstIterator<_Ty>& operator -= (int value) { ptr -= value; return *this; }
    /// Add an offset to the pointer.
    RandomAccessConstIterator<_Ty>& operator += (size_t value) { ptr += value; return *this; }
    /// Subtract an offset from the pointer.
    RandomAccessConstIterator<_Ty>& operator -= (size_t value) { ptr -= value; return *this; }
    /// Add an offset to the pointer.
    RandomAccessConstIterator<_Ty> operator + (int value) const { return RandomAccessConstIterator<_Ty>(ptr + value); }
    /// Subtract an offset from the pointer.
    RandomAccessConstIterator<_Ty> operator - (int value) const { return RandomAccessConstIterator<_Ty>(ptr - value); }
    /// Add an offset to the pointer.
    RandomAccessConstIterator<_Ty> operator + (size_t value) const { return RandomAccessConstIterator<_Ty>(ptr + value); }
    /// Subtract an offset from the pointer.
    RandomAccessConstIterator<_Ty> operator - (size_t value) const { return RandomAccessConstIterator<_Ty>(ptr - value); }
    /// Calculate offset to another iterator.
    int operator - (const RandomAccessConstIterator& rhs) const { return (int)(ptr - rhs.ptr); }
    /// Test for equality with another iterator.
    bool operator == (const RandomAccessConstIterator& rhs) const { return ptr == rhs.ptr; }
    /// Test for inequality with another iterator.
    bool operator != (const RandomAccessConstIterator& rhs) const { return ptr != rhs.ptr; }
    /// Test for less than with another iterator.
    bool operator < (const RandomAccessConstIterator& rhs) const { return ptr < rhs.ptr; }
    /// Test for greater than with another iterator.
    bool operator > (const RandomAccessConstIterator& rhs) const { return ptr > rhs.ptr; }
    /// Test for less than or equal with another iterator.
    bool operator <= (const RandomAccessConstIterator& rhs) const { return ptr <= rhs.ptr; }
    /// Test for greater than or equal with another iterator.
    bool operator >= (const RandomAccessConstIterator& rhs) const { return ptr >= rhs.ptr; }

    /// Pointer to the random-accessed object(s).
    _Ty* ptr;
};

}
