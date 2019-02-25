#pragma once

#include "../AutoConfig.h"

#include <cstddef>
#include <new>

namespace Auto3D
{

struct AllocatorBlock;
struct AllocatorNode;

/// %Allocator memory block.
struct AUTO_API AllocatorBlock
{
    /// Size of a node.
    size_t nodeSize;
    /// Number of nodes in this block.
    size_t capacity;
    /// First free node.
    AllocatorNode* free;
    /// Next allocator block.
    AllocatorBlock* next;
    /// Nodes follow.
};

/// %Allocator node.
struct AUTO_API AllocatorNode
{
    /// Next free node.
    AllocatorNode* next;
    /// Data follows.
};

/// Initialize a fixed-size allocator with the node size and initial capacity.
AUTO_API AllocatorBlock* AllocatorInitialize(size_t nodeSize, size_t initialCapacity = 1);
/// Uninitialize a fixed-size allocator. Frees all blocks in the chain.
AUTO_API void AllocatorUninitialize(AllocatorBlock* allocator);
/// Allocate a node. Creates a new block if necessary.
AUTO_API void* AllocatorGet(AllocatorBlock* allocator);
/// Free a node. Does not free any blocks.
AUTO_API void AllocatorFree(AllocatorBlock* allocator, void* node);

/// %Allocator template class. Allocates objects of a specific class.
template <class _Ty> class Allocator
{
public:
    /// Construct with optional initial capacity.
    Allocator(size_t capacity = 0) :
        allocator(nullptr)
    {
        if (capacity)
            Reserve(capacity);
    }
    
    /// Destruct. All objects reserved from this allocator should be freed before this is called.
    ~Allocator()
    {
        Reset();
    }
    
    /// Reserve initial capacity. Only possible before allocating the first object.
    void Reserve(size_t capacity)
    {
        if (!allocator)
            allocator = AllocatorInitialize(sizeof(_Ty), capacity);
    }

    /// Allocate and default-construct an object.
    _Ty* Allocate()
    {
        if (!allocator)
            allocator = AllocatorInitialize(sizeof(_Ty));
        _Ty* newObject = static_cast<_Ty*>(AllocatorGet(allocator));
        new(newObject) _Ty();
        
        return newObject;
    }
    
    /// Allocate and copy-construct an object.
    _Ty* Allocate(const _Ty& object)
    {
        if (!allocator)
            allocator = AllocatorInitialize(sizeof(_Ty));
        _Ty* newObject = static_cast<_Ty*>(AllocatorGet(allocator));
        new(newObject) _Ty(object);
        
        return newObject;
    }
    
    /// Destruct and free an object.
    void Free(_Ty* object)
    {
        (object)->~_Ty();
        AllocatorFree(allocator, object);
    }
    
    /// Free the allocator. All objects reserved from this allocator should be freed before this is called.
    void Reset()
    {
        AllocatorUninitialize(allocator);
        allocator = nullptr;
    }
    
private:
    /// Prevent copy construction.
    Allocator(const Allocator<_Ty>& rhs);
    /// Prevent assignment.
    Allocator<_Ty>& operator = (const Allocator<_Ty>& rhs);
    
    /// Allocator block.
    AllocatorBlock* allocator;
};

}
