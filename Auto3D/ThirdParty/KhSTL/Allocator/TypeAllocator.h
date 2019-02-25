#ifndef KH_STL_TYPEE_ALLCATOR_H_
#define KH_STL_TYPEE_ALLCATOR_H_
namespace KhSTL {

struct tAllocatorBlock;
struct tAllocatorNode;

/**
* Allocator memory block
*/
struct tAllocatorBlock
{
	/// Size of a node
	unsigned nodeSize;
	/// Number of nodes in this block
	unsigned capacity;
	/// First free node
	tAllocatorNode* free;
	/// Next allocator block
	tAllocatorBlock* next;
	/// Nodes follow
};


/**
* Allocator node
*/
struct tAllocatorNode
{
	/// Next free node
	tAllocatorNode* next;
	/// Data follows
};



static tAllocatorBlock* AllocatorReserveBlock(tAllocatorBlock* allocator, unsigned nodeSize, unsigned capacity)
{
	if (!capacity)
		capacity = 1;

	auto* blockPtr = new unsigned char[sizeof(tAllocatorBlock) + capacity * (sizeof(tAllocatorNode) + nodeSize)];
	auto* newBlock = reinterpret_cast<tAllocatorBlock*>(blockPtr);
	newBlock->nodeSize = nodeSize;
	newBlock->capacity = capacity;
	newBlock->free = nullptr;
	newBlock->next = nullptr;

	if (!allocator)
		allocator = newBlock;
	else
	{
		newBlock->next = allocator->next;
		allocator->next = newBlock;
	}

	// Initialize the nodes. Free nodes are always chained to the first (parent) allocator
	unsigned char* nodePtr = blockPtr + sizeof(tAllocatorBlock);
	auto* firstNewNode = reinterpret_cast<tAllocatorNode*>(nodePtr);

	for (unsigned i = 0; i < capacity - 1; ++i)
	{
		auto* newNode = reinterpret_cast<tAllocatorNode*>(nodePtr);
		newNode->next = reinterpret_cast<tAllocatorNode*>(nodePtr + sizeof(tAllocatorNode) + nodeSize);
		nodePtr += sizeof(tAllocatorNode) + nodeSize;
	}
	// i == capacity - 1
	{
		auto* newNode = reinterpret_cast<tAllocatorNode*>(nodePtr);
		newNode->next = nullptr;
	}

	allocator->free = firstNewNode;

	return newBlock;
}
/**
* @brief : Initialize a fixed-size allocator with the node size and initial capacity
*/
static tAllocatorBlock* AllocatorInitialize(unsigned nodeSize, unsigned initialCapacity = 1)
{
	tAllocatorBlock* block = AllocatorReserveBlock(nullptr, nodeSize, initialCapacity);
	return block;
}
/**
* @brief : Uninitialize a fixed-size allocator. Frees all blocks in the chain
*/
static void AllocatorUninitialize(tAllocatorBlock* allocator)
{
	while (allocator)
	{
		tAllocatorBlock* next = allocator->next;
		delete[] reinterpret_cast<unsigned char*>(allocator);
		allocator = next;
	}
}
/**
* @brief : Reserve a node. Creates a new block if necessary
*/
static void* AllocatorReserve(tAllocatorBlock* allocator)
{
	if (!allocator)
		return nullptr;

	if (!allocator->free)
	{
		// Free nodes have been exhausted. Allocate a new larger block
		unsigned newCapacity = (allocator->capacity + 1) >> 1u;
		AllocatorReserveBlock(allocator, allocator->nodeSize, newCapacity);
		allocator->capacity += newCapacity;
	}

	// We should have new free node(s) chained
	tAllocatorNode* freeNode = allocator->free;
	void* ptr = (reinterpret_cast<unsigned char*>(freeNode)) + sizeof(tAllocatorNode);
	allocator->free = freeNode->next;
	freeNode->next = nullptr;

	return ptr;
}
/**
* @brief : Free a node. Does not free any blocks
*/
static void AllocatorFree(tAllocatorBlock* allocator, void* ptr)
{
	if (!allocator || !ptr)
		return;

	auto* dataPtr = static_cast<unsigned char*>(ptr);
	auto* node = reinterpret_cast<tAllocatorNode*>(dataPtr - sizeof(tAllocatorNode));

	// Chain the node back to free nodes
	node->next = allocator->free;
	allocator->free = node;
}


/**
* Allocator template class. Allocates objects of a specific class
*/
template <class _Ty> 
	class tAllocator
{
public:
	using ValueType = _Ty;
public:
	/**
	* @brief : Construct default allocator (do nothing)
	*/
	constexpr tAllocator() noexcept
		: _allocator(nullptr)
	{
		_allocator = AllocatorInitialize((unsigned)sizeof(_Ty));
	}
	/**
	* @brief : Construct
	*/
	explicit tAllocator(unsigned initialCapacity) 
		: _allocator(nullptr)
	{
		if (initialCapacity)
			_allocator = AllocatorInitialize((unsigned)sizeof(_Ty), initialCapacity);
	}
	/**
	* @brief : Destruct
	*/
	virtual ~tAllocator()
	{
		AllocatorUninitialize(_allocator);
	}
	/**
	* @brief : Prevent copy construction
	*/
	tAllocator(const tAllocator<_Ty>& rhs) = delete;
	/**
	* @brief : Prevent assignment
	*/
	tAllocator<_Ty>& operator =(const tAllocator<_Ty>& rhs) = delete;

	/**
	* @brief : Reserve and default-construct an object
	*/
	_Ty* Reserve()
	{
		if (!_allocator)
			_allocator = AllocatorInitialize((unsigned)sizeof(_Ty));
		auto* newObject = static_cast<_Ty*>(AllocatorReserve(_allocator));
		new(newObject) _Ty();

		return newObject;
	}
	/**
	* @brief : Reserve and copy-construct an object
	*/
	_Ty* Reserve(const _Ty& object)
	{
		if (!_allocator)
			_allocator = AllocatorInitialize((unsigned)sizeof(_Ty));
		auto* newObject = static_cast<_Ty*>(AllocatorReserve(_allocator));
		new(newObject) _Ty(object);

		return newObject;
	}
	/**
	* @brief : Destruct and free an object
	*/
	void Free(_Ty* object)
	{
		(object)->~_Ty();
		AllocatorFree(_allocator, object);
	}
	/**
	* @brief : Destruct and free an other object
	*/
	template <typename _Other> void Free(_Other* object)
	{
		(object)->~_Other();
		AllocatorFree(_allocator, object);
	}
	/**
	* @brief : Allocate allocator
	*/
	void* Allocation()
	{
		return AllocatorReserve(_allocator);
	}

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

	template<class _Other>
	void Deallocate(_Other* buffer)
	{
		::operator delete(buffer);
	}
private:
	/// Allocator block
	tAllocatorBlock* _allocator;
};




}

#endif //!KH_STL_TYPEE_ALLCATOR_H_