#pragma once

#include "AutoConfig.h"

#include <cstddef>
#include <new>

namespace Auto3D
{

struct FAllocatorBlock;
struct FAllocatorNode;

/// %TAllocator memory block.
struct AUTO_API FAllocatorBlock
{
    /// Size of a node.
    size_t _nodeSize;
    /// Number of nodes in this block.
    size_t _capacity;
    /// First free node.
    FAllocatorNode* _free;
    /// Next allocator block.
    FAllocatorBlock* _next;
    /// Nodes follow.
};

/// %TAllocator node.
struct AUTO_API FAllocatorNode
{
    /// Next free node.
    FAllocatorNode* _next;
    /// Data follows.
};

static FAllocatorBlock* AllocatorReserveBlock(FAllocatorBlock* allocator, unsigned nodeSize, unsigned capacity)
{
	if (!capacity)
		capacity = 1;

	auto* blockPtr = new unsigned char[sizeof(FAllocatorBlock) + capacity * (sizeof(FAllocatorNode) + nodeSize)];
	auto* newBlock = reinterpret_cast<FAllocatorBlock*>(blockPtr);
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
	auto* firstNewNode = reinterpret_cast<FAllocatorNode*>(nodePtr);

	for (unsigned i = 0; i < capacity - 1; ++i)
	{
		auto* newNode = reinterpret_cast<FAllocatorNode*>(nodePtr);
		newNode->_next = reinterpret_cast<FAllocatorNode*>(nodePtr + sizeof(FAllocatorNode) + nodeSize);
		nodePtr += sizeof(FAllocatorNode) + nodeSize;
	}
	// i == capacity - 1
	{
		auto* newNode = reinterpret_cast<FAllocatorNode*>(nodePtr);
		newNode->_next = nullptr;
	}

	allocator->_free = firstNewNode;

	return newBlock;
}

/// Initialize a fixed-_size allocator with the node _size and initial capacity.
AUTO_API FAllocatorBlock* AllocatorInitialize(size_t nodeSize, size_t initialCapacity = 1);
/// Uninitialize a fixed-_size allocator. Frees all blocks in the chain.
AUTO_API void AllocatorUninitialize(FAllocatorBlock* allocator);
/// Allocate a node. Creates a new block if necessary.
AUTO_API void* AllocatorGet(FAllocatorBlock* allocator);
/// Free a node. Does not free any blocks.
AUTO_API void AllocatorFree(FAllocatorBlock* allocator, void* node);

template <typename _Ty>
class TAllocator
{
public:
	using ValueType = _Ty;
public:
	/// Construct default allocator (do nothing)
	constexpr TAllocator() noexcept
		: _allocator(nullptr)
	{
		_allocator = AllocatorInitialize((unsigned)sizeof(_Ty));
	}
	/// Construct
	explicit TAllocator(unsigned initialCapacity)
		: _allocator(nullptr)
	{
		if (initialCapacity)
			_allocator = AllocatorInitialize((unsigned)sizeof(_Ty), initialCapacity);
	}
	/// Destruct
	virtual ~TAllocator()
	{
		AllocatorUninitialize(_allocator);
	}
	/// Prevent copy construction
	TAllocator(const TAllocator<_Ty>& rhs) = delete;
	/// Prevent assignment
	TAllocator<_Ty>& operator =(const TAllocator<_Ty>& rhs) = delete;

	/// Reserve and default-construct an object
	_Ty* Reserve()
	{
		if (!_allocator)
			_allocator = AllocatorInitialize((unsigned)sizeof(_Ty));
		auto* newObject = static_cast<_Ty*>(AllocatorReserve(_allocator));
		new(newObject) _Ty();

		return newObject;
	}
	/// Reserve and copy-construct an object
	_Ty* Reserve(const _Ty& object)
	{
		if (!_allocator)
			_allocator = AllocatorInitialize((unsigned)sizeof(_Ty));
		auto* newObject = static_cast<_Ty*>(AllocatorReserve(_allocator));
		new(newObject) _Ty(object);

		return newObject;
	}
	/// Destruct and free an object
	void Free(_Ty* object)
	{
		(object)->~_Ty();
		AllocatorFree(_allocator, object);
	}
	/// Destruct and free an other object
	template <typename _Other> void Free(_Other* object)
	{
		(object)->~_Other();
		AllocatorFree(_allocator, object);
	}
	/// Allocate and default-construct an object.
	_Ty* Allocate()
	{
		if (!_allocator)
			_allocator = AllocatorInitialize(sizeof(_Ty));
		_Ty* newObject = static_cast<_Ty*>(AllocatorGet(_allocator));
		new(newObject) _Ty();

		return newObject;
	}

	/// Allocate and copy-construct an object.
	_Ty* Allocate(const _Ty& object)
	{
		if (!_allocator)
			_allocator = AllocatorInitialize(sizeof(_Ty));
		_Ty* newObject = static_cast<_Ty*>(AllocatorGet(_allocator));
		new(newObject) _Ty(object);

		return newObject;
	}
	/// Free the allocator. All objects reserved from this allocator should be freed before this is called.
	void Reset()
	{
		AllocatorUninitialize(_allocator);
		_allocator = nullptr;
	}
	/// Allocate allocator
	void* Allocation()
	{
		return AllocatorReserve(_allocator);
	}
	/// Allocate base
	template<typename _Other>
	_Other* Allocate(unsigned size)
	{
		std::set_new_handler(0);
		_Other* tmp = (_Other*)(::operator new((unsigned)(size * sizeof(_Other))));
		if (tmp == 0)
		{
			std::cerr << "out of memory" << std::endl;
			exit(1);
		}
		return tmp;
	}
	/// Deallocate
	template<class _Other>
	void Deallocate(_Other* buffer)
	{
		::operator delete(buffer);
	}
private:
	/// TAllocator block
	FAllocatorBlock* _allocator;
};

///Safe delete for cpp
#define SafeDelete(_X)	 do { if(_X){ delete (_X); (_X) = nullptr; } } while(0)
///Safe delete array for cpp 
#define SafeDeleteArray(_X) do { if(_X) { delete[] (_X); (_X) = nullptr; } } while(0)
///Safe free for c
#define SafeFree(_X)  do { if(_X) { free(_X); (_X) = nullptr; } } while(0)
///Safe release for c
#define SafeRelease(_X)  do { if(_X) { (_X)->release(); } } while(0)
///Safe release for c and initialize
#define SafeReleaseNull(_X) do { if(_X) { (_X)->release(); (_X) = nullptr; } } while(0)
///Safe retain for c
#define SafeRetain(_X)  do { if(_X) { (_X)->retain(); } } while(0)
///Break if
#define BreakIf(_X) do { if(_X) break; } while(0)

}
