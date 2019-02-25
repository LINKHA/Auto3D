#ifndef KH_STL_TYPE_VECTOR_ALLOCATOR_H_
#define KH_STL_TYPE_VECTOR_ALLOCATOR_H_

#include "../../Algorithm/TypeAlgorithm.h"

#include "TypeVectorValue.h"

#include"../../Allocator/TypeAllocator.h"

#include "../../Utility/TypeIterator.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:6293)
#endif

namespace KhSTL {
/**
* tpye vector Base class
*/
template <typename _Ty,
	typename _Alloc = tAllocator<_Ty>>
	class tVectorAlloc : public tVectorValue<_Ty>
{
public:
	using Iterator = tIterator<_Ty>;
	using ConstIterator = tConstIterator<_Ty>;
	using Value = tVectorValue<_Ty>;
	struct CopyTag {};
	struct MoveTag {};

public:
	tVectorAlloc() noexcept = default;

	/*tVectorAlloc()
		: _allocator(nullptr)
	{

	}*/
	virtual ~tVectorAlloc()
	{

	}
	
protected:
	/**
	* @brief : Construct elements using default ctor
	*/
	void constructElements(_Ty* dest, unsigned count)
	{
		for (unsigned i = 0; i < count; ++i)
			new(dest + i) _Ty();
	}
	/**
	* @brief : Copy-construct elements
	*/
	template <typename _RandomIterator>
	void constructElements(_Ty* dest, _RandomIterator start, _RandomIterator end, CopyTag)
	{
		const unsigned count = end - start;
		for (unsigned i = 0; i < count; ++i)
			new(dest + i) _Ty(*(start + i));
	}
	/**
	* @brief : Move-construct elements
	*/
	template <typename _RandomIterator>
	void constructElements(_Ty* dest, _RandomIterator start, _RandomIterator end, MoveTag)
	{
		const unsigned count = end - start;
		for (unsigned i = 0; i < count; ++i)
			new(dest + i) _Ty(std::move(*(start + i)));
	}
	/**
	* @brief : Calculate new vector capacity
	*/
	unsigned calculateCapacity(unsigned size, unsigned capacity)
	{
		if (!capacity)
			return size;
		else
		{
			while (capacity < size)
				capacity += (capacity + 1) >> 1;
			return capacity;
		}
	}
	
	
	/**
	* @brief : Call the elements' destructors
	*/
	void destructElements(_Ty* dest, unsigned count)
	{
		while (count--)
		{
			dest->~_Ty();
			++dest;
		}
	}


protected:
	_Alloc* _allocator;
};


}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif //!KH_STL_TYPE_VECTOR_ALLOCATOR_H_
