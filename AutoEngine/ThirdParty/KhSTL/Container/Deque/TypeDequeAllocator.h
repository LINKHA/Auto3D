#ifndef KH_STL_TYPE_DEQUE_ALLOCATOR_H_
#define KH_STL_TYPE_DEQUE_ALLOCATOR_H_

#include "TypeDequeValue.h"
#include <algorithm>
#include "../../Allocator/TypeAllocator.h"


namespace KhSTL {

template <typename _Ty
	, unsigned _Size
	, typename _Alloc>
class tDequeAlloc : public tDequeValue <_Ty, _Size>
{
public:
	using ValueType = _Ty;
	using Ptr = _Ty*;
	using MapPoint = _Ty**;
	using Value = tDequeValue <_Ty, _Size>;
	using This = tDequeAlloc<_Ty, _Size, _Alloc>;
	using Base = tDequeValue <_Ty, _Size>;
	using Iterator = typename Value::Iterator;
	using ConstIterator = typename Value::ConstIterator;
	using AllocType = typename _Alloc::ValueType;
public:
	tDequeAlloc()
	{}
	virtual ~tDequeAlloc()
	{}
protected:
	/**
	* @brief : Back-up when a buffer overflows
	*/
	void pushBackAux(const _Ty& value)
	{
		_Ty val = value;
		*(Value::_finish.node + 1) = allocateNode();
		//construct(Value::_finish.cur, val);
		new (Value::_finish.cur)_Ty(val);
		Value::_finish.SetNode(Value::_finish.node + 1);
		Value::_finish.cur = Value::_finish.first;
	}
	/**
	* @brief : Forward interpolation when a buffer overflows
	*/
	void pushFrontAux(const _Ty& value)
	{
		_Ty val = value;
		*(Value::_start.node - 1) = allocateNode();
		Value::_start.SetNode(Value::_start.node - 1);
		Value::_start.cur = Value::_start.last - 1;
		//construct(Value::_start.cur, val);
		new (Value::_start.cur)_Ty(val);
	}
	/**
	* @brief : Delete after buffer overflow
	*/
	void popBackAux()
	{
		freeNode(Value::_finish.first);
		Value::_finish.SetNode(Value::_finish.node - 1);
		Value::_finish.cur = Value::_finish.last - 1;
		//destroy(Value::_finish.cur);
		freeNode(Value::_finish.cur);
	}

	/**
	* @brief : Delete before buffer overflow
	*/
	void popFrontAux()
	{
		//destroy(Value::_start.cur);
		freeNode(Value::_finish.cur);
		freeNode(Value::_start.first);
		Value::_start.SetNode(Value::_start.node + 1);
		Value::_start.cur = Value::_start.first;
	}
	/**
	* @brief : Replication function
	*/
	void * __cdecl memmove(void * dst, const void * src, size_t count)
	{
		void * ret = dst;
		if (dst <= src || (char *)dst >= ((char *)src + count))
		{
			// If the dst and src regions do not overlap, 
			// copy one by one from the beginning
			while (count--)
			{
				*(char *)dst = *(char *)src;
				dst = (char *)dst + 1;
				src = (char *)src + 1;
			}
		}
		else
		{
			// If the dst and src regions cross, you copy from the tail to 
			// the start position to avoid data conflicts
			dst = (char *)dst + count - 1;
			src = (char *)src + count - 1;
			while (count--)
			{
				*(char *)dst = *(char *)src;
				dst = (char *)dst - 1;
				src = (char *)src - 1;
			}
		}
		return(ret);
	}
	int bufferSize()
	{
		return _Size != 0 ? _Size : (sizeof(_Ty) < 512 ? int(512 / sizeof(_Ty)) : int(1));
	}
	/**
	* @brief : Various initialization
	*/
	void fillInitialize(unsigned num, const _Ty& value)
	{
		createMapAndNodes(num);
		MapPoint cur;
		for (cur = Value::_start.node; cur < Value::_finish.node; ++cur)
		{
			std::uninitialized_fill(*cur, *cur + bufferSize(), value);
		}
		std::uninitialized_fill(Value::_finish.first, Value::_finish.cur, value);
	}
	/**
	* @brief : Initialize the buffer size managed by each node in the (map), control center
	*/
	void createMapAndNodes(unsigned numElements)
	{
		unsigned numNodes = numElements / bufferSize() + 1;
		Value::_mapSize = std::max(initialMapSize(), numNodes + 2);
		Value::_map = allocateMap(Value::_mapSize);//allocateMap problem
		MapPoint nstart = Value::_map + (Value::_mapSize - numNodes) / 2;
		MapPoint nfinish = nstart + numNodes - 1;
		MapPoint cur;
		for (cur = nstart; cur <= nfinish; ++cur)
		{
			*cur = allocateNode();
		}
		Value::_start.SetNode(nstart);
		Value::_finish.SetNode(nfinish);
		Value::_start.cur = Value::_start.first;
		Value::_finish.cur = Value::_finish.first + numElements % bufferSize();
	}
	/**
	* @brief : Minimum number of (map) nodes in Control and Control Center
	*/
	unsigned initialMapSize() { return (unsigned)8; }
	
	/**
	* @brief : Release the buffer for the control center ( map ) node
	*/
	void freeNode(_Ty* node)
	{
		_allocator.Deallocate(node);
	}


	_Ty* allocate(unsigned n)
	{
		std::set_new_handler(0);
		_Ty* tmp = (_Ty*)(::operator new((unsigned)(n * sizeof(_Ty))));
		if (tmp == 0)
		{
			std::cerr << "out of memory" << std::endl;
			exit(1);
		}
		return tmp;
	}
	/**
	* @brief : Configure the buffer size of each node in the control center (map)
	*/
	_Ty* allocateNode()
	{
		return (_Ty*)allocate(bufferSize());
	}

	/**
	* @brief : Configure the buffer size of each node in the control center (map)
	*/
	_Ty** allocateMap(unsigned n)
	{
		return (_Ty**)allocate(n * sizeof(_Ty*));
	}

private:
	AllocType* reserve()
	{
		return _allocator.Reserve();
	}
	/**
	* @brief : Reserve and copy-construct an object
	*/
	AllocType* reserve(const AllocType& object)
	{
		return _allocator.Reserve(object);
	}
	/**
	* @brief : Destruct and free an object
	*/
	void free(AllocType* object)
	{
		_allocator.Free(object);
	}

	void* allocation()
	{
		return _allocator.Allocation();
	}
protected:
	_Alloc _allocator;
};

}


#endif // !KH_STL_TYPE_DEQUE_ALLOCATOR_H_