#include "Allocator.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

static AllocatorBlock* AllocatorGetBlock(AllocatorBlock* allocator, size_t nodeSize, size_t capacity)
{
    if (!capacity)
        capacity = 1;
    
    unsigned char* blockPtr = new unsigned char[sizeof(AllocatorBlock) + capacity * (sizeof(AllocatorNode) + nodeSize)];
    AllocatorBlock* newBlock = reinterpret_cast<AllocatorBlock*>(blockPtr);
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
    unsigned char* nodePtr = blockPtr + sizeof(AllocatorBlock);
    AllocatorNode* firstNewNode = reinterpret_cast<AllocatorNode*>(nodePtr);
    
    for (size_t i = 0; i < capacity - 1; ++i)
    {
        AllocatorNode* newNode = reinterpret_cast<AllocatorNode*>(nodePtr);
        newNode->_next = reinterpret_cast<AllocatorNode*>(nodePtr + sizeof(AllocatorNode) + nodeSize);
        nodePtr += sizeof(AllocatorNode) + nodeSize;
    }
    // i == capacity - 1
    {
        AllocatorNode* newNode = reinterpret_cast<AllocatorNode*>(nodePtr);
        newNode->_next = nullptr;
    }
    
    allocator->_free = firstNewNode;
    return newBlock;
}

AllocatorBlock* AllocatorInitialize(size_t nodeSize, size_t initialCapacity)
{
    AllocatorBlock* block = AllocatorGetBlock(nullptr, nodeSize, initialCapacity);
    return block;
}

void AllocatorUninitialize(AllocatorBlock* allocator)
{
    while (allocator)
    {
        AllocatorBlock* next = allocator->_next;
        delete[] reinterpret_cast<unsigned char*>(allocator);
        allocator = next;
    }
}

void* AllocatorGet(AllocatorBlock* allocator)
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
    AllocatorNode* freeNode = allocator->_free;
    void* ptr = (reinterpret_cast<unsigned char*>(freeNode)) + sizeof(AllocatorNode);
    allocator->_free = freeNode->_next;
    freeNode->_next = nullptr;
    
    return ptr;
}

void AllocatorFree(AllocatorBlock* allocator, void* ptr)
{
    if (!allocator || !ptr)
        return;
    
    unsigned char* dataPtr = static_cast<unsigned char*>(ptr);
    AllocatorNode* node = reinterpret_cast<AllocatorNode*>(dataPtr - sizeof(AllocatorNode));
    
    // Chain the node back to free nodes
    node->_next = allocator->_free;
    allocator->_free = node;
}

}
