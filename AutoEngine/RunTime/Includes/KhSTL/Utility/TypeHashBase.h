#ifndef KH_STL_TYPE_HASH_BASE_H_
#define KH_STL_TYPE_HASH_BASE_H_
#include "../Allocator/TypeAllocator.h"
#include "../Algorithm/TypeAlgorithm.h"

namespace KhSTL {

/**
* Hash set/map node Base class
*/
struct tHashNodeBase
{
	/**
	* @brief : Construct
	*/
	tHashNodeBase()
		: down(nullptr)
		, prev(nullptr)
		, next(nullptr)
	{}

	/// Next node in the bucket.
	tHashNodeBase* down;
	/// Previous node.
	tHashNodeBase* prev;
	/// Next node.
	tHashNodeBase* next;
};

/**
* Hash set/map iterator Base class
*/
struct tHashIteratorBase
{
	/**
	* @brief : Construct
	*/
	tHashIteratorBase()
		: ptr(nullptr)
	{}
	/**
	* @brief : Construct with a node pointer
	*/
	explicit tHashIteratorBase(tHashNodeBase* ptr)
		: ptr(ptr)
	{}
	/**
	* @brief : Test for equality with another iterator
	*/
	bool operator ==(const tHashIteratorBase& rhs) const { return ptr == rhs.ptr; }
	/**
	* @brief : Test for inequality with another iterator
	*/
	bool operator !=(const tHashIteratorBase& rhs) const { return ptr != rhs.ptr; }
	/**
	* @brief : Go to the next node
	*/
	void GotoNext()
	{
		if (ptr)
			ptr = ptr->next;
	}
	/**
	* @brief : Go to the previous node
	*/
	void GotoPrev()
	{
		if (ptr)
			ptr = ptr->prev;
	}

	/// Node pointer
	tHashNodeBase* ptr;
};

class tHashBase 
{
public:
	/// Initial amount of buckets
	static const unsigned MIN_BUCKETS = 8;
	/// Maximum load factor
	static const unsigned MAX_LOAD_FACTOR = 4;
public:
	/**
	* @brief : Construct
	*/
	tHashBase() 
		: _head(nullptr)
		, _tail(nullptr)
		, _ptrs(nullptr)
		, _allocator(nullptr)
	{}
	/**
	* @brief : Swap with another hash set or map
	*/
	void Swap(tHashBase& rhs)
	{
		KhSTL::Swap(_head, rhs._head);
		KhSTL::Swap(_tail, rhs._tail);
		KhSTL::Swap(_ptrs, rhs._ptrs);
		KhSTL::Swap(_allocator, rhs._allocator);
	}
	/**
	* @brief : Return number of elements
	*/
	unsigned Size() const { return _ptrs ? (reinterpret_cast<unsigned*>(_ptrs))[0] : 0; }
	/**
	* @brief : Return number of buckets
	*/
	unsigned NumBuckets() const { return _ptrs ? (reinterpret_cast<unsigned*>(_ptrs))[1] : 0; }
	/**
	* @brief : Return whether has no elements
	*/
	bool Empty() const { return Size() == 0; }
protected:
	/**
	* @brief : Allocate bucket head pointers + room for size and bucket count variables
	*/
	void allocateBuckets(unsigned size, unsigned numBuckets)
	{
		delete[] _ptrs;

		auto ptrs = new tHashNodeBase*[numBuckets + 2];
		auto* data = reinterpret_cast<unsigned*>(ptrs);
		data[0] = size;
		data[1] = numBuckets;
		_ptrs = ptrs;

		resetPtrs();
	}
	/**
	* @brief : Reset bucket head pointers
	*/
	void resetPtrs()
	{
		// Reset bucket pointers
		if (!_ptrs)
			return;

		unsigned numBuckets = NumBuckets();
		tHashNodeBase** tptrs = ptrs();
		for (unsigned i = 0; i < numBuckets; ++i)
			tptrs[i] = nullptr;
	}
	/**
	* @brief : Set new size
	*/
	void setSize(unsigned size) { if (_ptrs) (reinterpret_cast<unsigned*>(_ptrs))[0] = size; }
	/**
	* @brief : Return bucket head pointers
	*/
	tHashNodeBase** ptrs() const { return _ptrs ? _ptrs + 2 : nullptr; }

	/// List head node pointer.
	tHashNodeBase* _head;
	/// List tail node pointer.
	tHashNodeBase* _tail;
	/// Bucket head pointers.
	tHashNodeBase** _ptrs;
	/// Node allocator.
	tAllocatorBlock* _allocator;

};




}
#endif //!KH_STL_TYPE_HASH_BASE_H_