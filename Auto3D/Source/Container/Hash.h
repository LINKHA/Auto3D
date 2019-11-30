#pragma once

#include "AutoConfig.h"

#include <cstddef>

namespace Auto3D
{

struct FAllocatorBlock;

/// Pointer hash function.
template <typename _Ty> unsigned MakeHash(_Ty* value)
{
    return ((unsigned)(size_t)value) / sizeof(_Ty);
}

/// Const pointer hash function.
template <typename _Ty> unsigned MakeHash(const _Ty* value)
{
    return ((unsigned)(size_t)value) / sizeof(_Ty);
}

/// Generic hash function.
template <typename _Ty> unsigned MakeHash(const _Ty& value)
{
    return value.ToHash();
}

/// Void pointer hash function.
template<> inline unsigned MakeHash(void* value)
{
    return (unsigned)(size_t)value;
}

/// Const void pointer hash function.
template<> inline unsigned MakeHash(const void* value)
{
    return (unsigned)(size_t)value;
}

/// Long long hash function.
template<> inline unsigned MakeHash(const long long& value)
{
    return (value >> 32) | (value & 0xffffffff);
}

/// Unsigned long long hash function.
template<> inline unsigned MakeHash(const unsigned long long& value)
{
    return (value >> 32) | (value & 0xffffffff);
}

/// Int hash function.
template<> inline unsigned MakeHash(const int& value)
{
    return value;
}

/// Unsigned hash function.
template<> inline unsigned MakeHash(const unsigned& value)
{
    return value;
}

/// Short hash function.
template<> inline unsigned MakeHash(const short& value)
{
    return value;
}

/// Unsigned short hash function.
template<> inline unsigned MakeHash(const unsigned short& value)
{
    return value;
}

/// Char hash function.
template<> inline unsigned MakeHash(const char& value)
{
    return value;
}

/// Unsigned char hash function.
template<> inline unsigned MakeHash(const unsigned char& value)
{
    return value;
}

/// Hash set/map node base class.
struct AUTO_API FHashNodeBase
{
    /// Construct.
    FHashNodeBase() :
        _down(nullptr),
        _prev(nullptr),
        _next(nullptr)
    {
    }
    
    /// Next node in the bucket.
    FHashNodeBase* _down;
    /// Previous node.
    FHashNodeBase* _prev;
    /// Next node.
    FHashNodeBase* _next;
};

/// Hash set/map iterator base class.
struct AUTO_API FHashIteratorBase
{
    /// Construct.
    FHashIteratorBase() :
        _ptr(nullptr)
    {
    }
    
    /// Construct with a node pointer.
    explicit FHashIteratorBase(FHashNodeBase* ptr) :
        _ptr(ptr)
    {
    }
    
    /// Test for equality with another iterator.
    bool operator == (const FHashIteratorBase& rhs) const { return _ptr == rhs._ptr; }
    /// Test for inequality with another iterator.
    bool operator != (const FHashIteratorBase& rhs) const { return _ptr != rhs._ptr; }
    
    /// Go to the next node.
    void GotoNext()
    {
        if (_ptr)
            _ptr = _ptr->_next;
    }
    
    /// Go to the previous node.
    void GotoPrev()
    {
        if (_ptr)
            _ptr = _ptr->_prev;
    }
    
    /// %ANode pointer.
    FHashNodeBase* _ptr;
};

/// Hash set/map base class.
class AUTO_API FHashBase
{
public:
    /// Initial amount of buckets.
    static const size_t MIN_BUCKETS = 8;
    /// Maximum load factor.
    static const size_t MAX_LOAD_FACTOR = 4;
    
    /// Construct.
    FHashBase() :
        _ptrs(nullptr),
        _allocator(nullptr)
    {
    }

    /// Destruct.
    ~FHashBase()
    {
        delete[] _ptrs;
    }
    
    /// Swap with another hash set or map.
    void Swap(FHashBase& hash);

    /// Return number of elements.
    size_t Size() const { return _ptrs ? (reinterpret_cast<size_t*>(_ptrs))[0] : 0; }
    /// Return whether has no elements.
    bool IsEmpty() const { return Size() == 0; }
    
protected:
    /// Allocate bucket head pointers + room for _size and bucket count variables.
    void AllocateBuckets(size_t _size, size_t numBuckets);
    /// Reset bucket head pointers.
    void ResetPtrs();
    /// Set new _size.
    void SetSize(size_t _size) { reinterpret_cast<size_t*>(_ptrs)[0] = _size; }
    /// Set new head node.
    void SetHead(FHashNodeBase* head) { _ptrs[2] = head; }
    /// Set new tail node.
    void SetTail(FHashNodeBase* tail) { _ptrs[3] = tail; }

    /// Return number of buckets.
    size_t NumBuckets() const { return _ptrs ? (reinterpret_cast<size_t*>(_ptrs))[1] : MIN_BUCKETS; }
    /// Return list head node.
    FHashNodeBase* Head() const { return _ptrs ? _ptrs[2] : nullptr; }
    /// Return list tail node.
    FHashNodeBase* Tail() const { return _ptrs ? _ptrs[3] : nullptr; }
    /// Return bucket head pointers.
    FHashNodeBase** Ptrs() const { return _ptrs ? _ptrs + 4 : nullptr; }
    
    /// Bucket head pointers.
    FHashNodeBase** _ptrs;
    /// %ANode allocator.
    FAllocatorBlock* _allocator;
};

}
