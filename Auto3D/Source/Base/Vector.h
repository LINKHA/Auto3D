#pragma once

#include "Iterator.h"
#include "Swap.h"

#include <cassert>
#include <cstring>
#include <new>

#ifdef _MSC_VER
#pragma warning(disable:4345)
#endif

namespace Auto3D
{

/// %Vector base class.
class AUTO_API VectorBase
{
public:
    /// Construct.
    VectorBase();

    /// Swap with another vector.
    void Swap(VectorBase& vector);

    /// Return number of elements in the vector.
    size_t Size() const { return buffer ? reinterpret_cast<size_t*>(buffer)[0] : 0; }
    /// Return element capacity of the vector.
    size_t Capacity() const { return buffer ? reinterpret_cast<size_t*>(buffer)[1] : 0; }
    /// Return whether has no elements.
    bool IsEmpty() const { return Size() == 0; }

protected:
    /// Set new _size.
    void SetSize(size_t _size) { reinterpret_cast<size_t*>(buffer)[0] = _size; } 
    /// Set new capacity.
    void SetCapacity(size_t capacity) { reinterpret_cast<size_t*>(buffer)[1] = capacity; } 

    /// Allocate the buffer for elements.
    static unsigned char* AllocateBuffer(size_t _size);

    /// Buffer. Contains _size and capacity in the beginning.
    unsigned char* buffer;
};

/// %Vector template class. Implements a dynamic-sized array where the elements are in continuous memory. The elements need to be safe to move with block copy.
template <class _Ty> class Vector : public VectorBase
{
public:
    typedef RandomAccessIterator<_Ty> Iterator;
    typedef RandomAccessConstIterator<_Ty> ConstIterator;

    /// Construct empty.
    Vector()
    {
    }

    /// Construct with initial _size.
    explicit Vector(size_t startSize)
    {
        Resize(startSize, 0);
    }

    /// Construct with initial data.
    Vector(const _Ty* _data, size_t dataSize)
    {
        Resize(dataSize, _data);
    }

    /// Copy-construct.
    Vector(const Vector<_Ty>& vector)
    {
        *this = vector;
    }

    /// Destruct.
    ~Vector()
    {
        DestructElements(Buffer(), Size());
        delete[] buffer;
    }

    /// Assign from another vector.
    Vector<_Ty>& operator = (const Vector<_Ty>& rhs)
    {
        if (&rhs != this)
        {
            Clear();
            Resize(rhs.Size(), rhs.Buffer());
        }
        return *this;
    }

    /// Add-assign an element.
    Vector<_Ty>& operator += (const _Ty& rhs)
    {
        Push(rhs);
        return *this;
    }

    /// Add-assign another vector.
    Vector<_Ty>& operator += (const Vector<_Ty>& rhs)
    {
        Push(rhs);
        return *this;
    }

    /// Add an element.
    Vector<_Ty> operator + (const _Ty& rhs) const
    {
        Vector<_Ty> ret(*this);
        ret.Push(rhs);
        return ret;
    }

    /// Add another vector.
    Vector<_Ty> operator + (const Vector<_Ty>& rhs) const
    {
        Vector<_Ty> ret(*this);
        ret.Push(rhs);
        return ret;
    }

    /// Test for equality with another vector.
    bool operator == (const Vector<_Ty>& rhs) const
    {
        size_t _size = Size();
        if (rhs.Size() != _size)
            return false;

        _Ty* lhsBuffer = Buffer();
        _Ty* rhsBuffer = rhs.Buffer();

        for (size_t i = 0; i < _size; ++i)
        {
            if (lhsBuffer[i] != rhsBuffer[i])
                return false;
        }

        return true;
    }

    /// Test for inequality with another vector.
    bool operator != (const Vector<_Ty>& rhs) const { return !(*this == rhs); }
    /// Return element at index.
    _Ty& operator [] (size_t index) { assert(index < Size()); return Buffer()[index]; }
    /// Return const element at index.
    const _Ty& operator [] (size_t index) const { assert(index < Size()); return Buffer()[index]; }

    /// Add an element at the end.
    void Push(const _Ty& value) { Resize(Size() + 1, &value); }
    /// Add another vector at the end.
    void Push(const Vector<_Ty>& vector) { Resize(Size() + vector.Size(), vector.Buffer()); }

    /// Remove the last element.
    void Pop()
    {
        if (Size())
            Resize(Size() - 1, 0);
    }

    /// Insert an element at _position.
    void Insert(size_t pos, const _Ty& value)
    {
        if (pos > Size())
            pos = Size();

        size_t oldSize = Size();
        Resize(Size() + 1, 0);
        MoveRange(pos + 1, pos, oldSize - pos);
        Buffer()[pos] = value;
    }

    /// Insert another vector at _position.
    void Insert(size_t pos, const Vector<_Ty>& vector)
    {
        if (pos > Size())
            pos = Size();

        size_t oldSize = Size();
        Resize(Size() + vector.Size(), 0);
        MoveRange(pos + vector.Size(), pos, oldSize - pos);
        CopyElements(Buffer() + pos, vector.Buffer(), vector.Size());
    }

    /// Insert an element by iterator.
    Iterator Insert(const Iterator& dest, const _Ty& value)
    {
        size_t pos = dest - Begin();
        if (pos > Size())
            pos = Size();
        Insert(pos, value);

        return Begin() + pos;
    }

    /// Insert a vector by iterator.
    Iterator Insert(const Iterator& dest, const Vector<_Ty>& vector)
    {
        size_t pos = dest - Begin();
        if (pos > Size())
            pos = Size();
        Insert(pos, vector);

        return Begin() + pos;
    }

    /// Insert a vector partially by iterators.
    Iterator Insert(const Iterator& dest, const ConstIterator& start, const ConstIterator& end)
    {
        size_t pos = dest - Begin();
        if (pos > Size())
            pos = Size();
        size_t length = end - start;
        Resize(Size() + length, 0);
        MoveRange(pos + length, pos, Size() - pos - length);

        _Ty* destPtr = Buffer() + pos;
        for (Iterator it = start; it != end; ++it)
            *destPtr++ = *it;

        return Begin() + pos;
    }

    /// Insert elements.
    Iterator Insert(const Iterator& dest, const _Ty* start, const _Ty* end)
    {
        size_t pos = dest - Begin();
        if (pos > Size())
            pos = Size();
        size_t length = end - start;
        Resize(Size() + length, 0);
        MoveRange(pos + length, pos, Size() - pos - length);

        _Ty* destPtr = Buffer() + pos;
        for (const _Ty* i = start; i != end; ++i)
            *destPtr++ = *i;

        return Begin() + pos;
    }

    /// Erase a range of elements.
    void Erase(size_t pos, size_t length = 1)
    {
        // Return if the range is illegal
        if (pos + length > Size() || !length)
            return;

        MoveRange(pos, pos + length, Size() - pos - length);
        Resize(Size() - length, 0);
    }

    /// Erase an element by iterator. Return iterator to the next element.
    Iterator Erase(const Iterator& it)
    {
        size_t pos = it - Begin();
        if (pos >= Size())
            return End();
        Erase(pos);

        return Begin() + pos;
    }

    /// Erase a range by iterators. Return iterator to the next element.
    Iterator Erase(const Iterator& start, const Iterator& end)
    {
        size_t pos = start - Begin();
        if (pos >= Size())
            return End();
        size_t length = end - start;
        Erase(pos, length);

        return Begin() + pos;
    }

    /// Erase an element if found.
    bool Remove(const _Ty& value)
    {
        Iterator it = Find(value);
        if (it != End())
        {
            Erase(it);
            return true;
        }
        else
            return false;
    }

    /// Clear the vector.
    void Clear() { Resize(0); }
    /// Resize the vector.
    void Resize(size_t newSize) { Resize(newSize, nullptr); }

    /// Set new capacity.
    void Reserve(size_t newCapacity)
    {
        size_t _size = Size();
        if (newCapacity < _size)
            newCapacity = _size;

        if (newCapacity != Capacity())
        {
            unsigned char* newBuffer = nullptr;
            
            if (newCapacity)
            {
                newBuffer = AllocateBuffer(newCapacity * sizeof(_Ty));
                // Move the data into the new buffer
                // This assumes the elements are safe to move without copy-constructing and deleting the old elements;
                // ie. they should not contain pointers to self, or interact with outside objects in their constructors
                // or destructors
                MoveElements(reinterpret_cast<_Ty*>(newBuffer + 2 * sizeof(size_t)), Buffer(), _size);
            }

            // Delete the old buffer
            delete[] buffer;
            buffer = newBuffer;
            SetSize(_size);
            SetCapacity(newCapacity);
        }
    }

    /// Reallocate so that no extra memory is used.
    void Compact() { Reserve(Size()); }

    /// Return element at index.
    _Ty& At(size_t index) { assert(index < Size()); return Buffer()[index]; }
    /// Return const element at index.
    const _Ty& At(size_t index) const { assert(index < Size()); return Buffer()[index]; }

    /// Return iterator to first occurrence of value, or to the end if not found.
    Iterator Find(const _Ty& value)
    {
        Iterator it = Begin();
        while (it != End() && *it != value)
            ++it;
        return it;
    }

    /// Return const iterator to first occurrence of value, or to the end if not found.
    ConstIterator Find(const _Ty& value) const
    {
        ConstIterator it = Begin();
        while (it != End() && *it != value)
            ++it;
        return it;
    }

    /// Return whether contains a specific value.
    bool Contains(const _Ty& value) const { return Find(value) != End(); }
    /// Return iterator to the beginning.
    Iterator Begin() { return Iterator(Buffer()); }
    /// Return const iterator to the beginning.
    ConstIterator Begin() const { return ConstIterator(Buffer()); }
    /// Return iterator to the end.
    Iterator End() { return Iterator(Buffer() + Size()); }
    /// Return const iterator to the end.
    ConstIterator End() const { return ConstIterator(Buffer() + Size()); }
    /// Return first element.
    _Ty& Front() { assert(Size()); return Buffer()[0]; }
    /// Return const first element.
    const _Ty& Front() const { assert(Size()); return Buffer()[0]; }
    /// Return last element.
    _Ty& Back() { assert(Size()); return Buffer()[Size() - 1]; }
    /// Return const last element.
    const _Ty& Back() const { assert(Size()); return Buffer()[Size() - 1]; }

private:
    /// Return the buffer with right type.
    _Ty* Buffer() const { return reinterpret_cast<_Ty*>(buffer + 2 * sizeof(size_t)); }

   /// Resize the vector and create/remove new elements as necessary.
    void Resize(size_t newSize, const _Ty* src)
    {
        size_t _size = Size();
        
        if (newSize < _size)
        {
            DestructElements(Buffer() + newSize, _size - newSize);
            SetSize(newSize);
        }
        else if (newSize > _size)
        {
            size_t capacity = Capacity();
            unsigned char* oldBuffer = nullptr;

            // Allocate new buffer if necessary and copy the current elements
            if (newSize > capacity)
            {
                if (!capacity)
                    capacity = newSize;
                else
                {
                    while (capacity < newSize)
                        capacity += (capacity + 1) >> 1;
                }

                oldBuffer = buffer;
                buffer = AllocateBuffer(capacity * sizeof(_Ty));
                if (oldBuffer)
                    MoveElements(Buffer(), reinterpret_cast<_Ty*>(oldBuffer + 2 * sizeof(size_t)), _size);

                SetCapacity(capacity);
            }

            // Initialize the new elements. Optimize for case of 1 element with source (push)
            size_t count = newSize - _size;
            _Ty* dest = Buffer() + _size;
            if (src)
            {
                if (count == 1)
                    new(dest) _Ty(*src);
                else
                {
                    _Ty* end = dest + count;
                    while (dest != end)
                    {
                        new(dest) _Ty(*src);
                        ++dest;
                        ++src;
                    }
                }
            }
            else
            {
                _Ty* end = dest + count;
                while (dest != end)
                {
                    new(dest) _Ty();
                    ++dest;
                }
            }

            SetSize(newSize);

            // Delete old buffer last
            delete[] oldBuffer;
        }
    }

    /// Move a range of elements within the vector.
    void MoveRange(size_t dest, size_t src, size_t count)
    {
        _Ty* _data = Buffer();
        if (src < dest)
        {
            for (size_t i = count - 1; i < count; --i)
                _data[dest + i] = _data[src + i];
        }
        if (src > dest)
        {
            for (size_t i = 0; i < count; ++i)
                _data[dest + i] = _data[src + i];
        }
    }

    /// Copy elements from one buffer to another.
    static void CopyElements(_Ty* dest, const _Ty* src, size_t count)
    {
        while (count--)
            *dest++ = *src++;
    }

    /// Move elements from one buffer to another. Constructors or destructors are not called.
    static void MoveElements(_Ty* dest, const _Ty* src, size_t count)
    {
        if (count)
            memcpy(dest, src, sizeof(_Ty) * count);
    }

    // Call the elements' destructors.
    static void DestructElements(_Ty* dest, size_t count)
    {
        while (count--)
        {
            dest->~_Ty();
            ++dest;
        }
    }
};

}
