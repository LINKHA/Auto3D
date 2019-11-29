#include "Allocator.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

static FAllocatorBlock* AllocatorGetBlock(FAllocatorBlock* allocator, size_t nodeSize, size_t capacity)
{
    if (!capacity)
        capacity = 1;
    
    unsigned char* blockPtr = new unsigned char[sizeof(FAllocatorBlock) + capacity * (sizeof(FAllocatorNode) + nodeSize)];
    FAllocatorBlock* newBlock = reinterpret_cast<FAllocatorBlock*>(blockPtr);
    newBlock->_nodeSize = nodeSize;
    newBlock->_capacity = capacity;
    newBlock->_free = nullptr;
    newBlock->_next = nullptr;
    
    if (!allocator)
        allocator = newBlock;
    else
    {
        newBlock->_next = allocator->_next;
        allocator->_next = newBlock;
    }
    
    // Initialize the nodes. Free nodes are always chained to the first (parent) allocator
    unsigned char* nodePtr = blockPtr + sizeof(FAllocatorBlock);
    FAllocatorNode* firstNewNode = reinterpret_cast<FAllocatorNode*>(nodePtr);
    
    for (size_t i = 0; i < capacity - 1; ++i)
    {
        FAllocatorNode* newNode = reinterpret_cast<FAllocatorNode*>(nodePtr);
        newNode->_next = reinterpret_cast<FAllocatorNode*>(nodePtr + sizeof(FAllocatorNode) + nodeSize);
        nodePtr += sizeof(FAllocatorNode) + nodeSize;
    }
    // i == capacity - 1
    {
        FAllocatorNode* newNode = reinterpret_cast<FAllocatorNode*>(nodePtr);
        newNode->_next = nullptr;
    }
    
    allocator->_free = firstNewNode;
    return newBlock;
}

FAllocatorBlock* AllocatorInitialize(size_t nodeSize, size_t initialCapacity)
{
    FAllocatorBlock* block = AllocatorGetBlock(nullptr, nodeSize, initialCapacity);
    return block;
}

void AllocatorUninitialize(FAllocatorBlock* allocator)
{
    while (allocator)
    {
        FAllocatorBlock* next = allocator->_next;
        delete[] reinterpret_cast<unsigned char*>(allocator);
        allocator = next;
    }
}

void* AllocatorGet(FAllocatorBlock* allocator)
{
    if (!allocator)
        return nullptr;
    
    if (!allocator->_free)
    {
        // Free nodes have been exhausted. Allocate a new larger block
        size_t newCapacity = (allocator->_capacity + 1) >> 1;
        AllocatorGetBlock(allocator, allocator->_nodeSize, newCapacity);
        allocator->_capacity += newCapacity;
    }
    
    // We should have new free node(s) chained
    FAllocatorNode* freeNode = allocator->_free;
    void* ptr = (reinterpret_cast<unsigned char*>(freeNode)) + sizeof(FAllocatorNode);
    allocator->_free = freeNode->_next;
    freeNode->_next = nullptr;
    
    return ptr;
}

void AllocatorFree(FAllocatorBlock* allocator, void* ptr)
{
    if (!allocator || !ptr)
        return;
    
    unsigned char* dataPtr = static_cast<unsigned char*>(ptr);
    FAllocatorNode* node = reinterpret_cast<FAllocatorNode*>(dataPtr - sizeof(FAllocatorNode));
    
    // Chain the node back to free nodes
    node->_next = allocator->_free;
    allocator->_free = node;
}

}
