#ifndef KH_STL_TYPE_HEAP_H_
#define KH_STL_TYPE_HEAP_H_
#include "../../Allocator/TypeAllocator.h"
namespace KhSTL{

template<typename _Ty
	,typename _Alloc = tAllocator<_Ty>>
class tHeap
{
public:
	using ValueType = _Ty;
	using Iterator = ValueType*;
public:
	/**
	* @brief : Construct empty
	*/
	tHeap()
		: _start(0)
		, _finish(0)
		, _endOfStorage(0) 
	{} 
	/**
	* @brief : Construct for num value
	*/
	tHeap(int size, const _Ty& value)
	{ 
		fillInitialize(size, value);
	} 
	/**
	* @brief : Construct for num value
	*/
	tHeap(long size, const _Ty& value)
	{ 
		fillInitialize(size, value);
	}
	/**
	* @brief : Initialize the length of the heap to num
	*/
	explicit tHeap(unsigned size)
	{ 
		fillInitialize(size, _Ty());
	}
	/**
	* @brief : Destructing structure
	*/
	~tHeap()
	{
		destroy(_start, _finish);
		deallocate();
	}

	Iterator Begin() { return _start; }
	Iterator End() { return _finish; }
	unsigned Size() { return unsigned(int(End() - Begin())); }

	void PushBack(const _Ty& value)
	{
		if (_finish != _endOfStorage)
		{
			construct(_finish, value);
			++_finish;
		}
		else
		{
			insertAux(End(), value);
		}
	}
	void PopBack()
	{
		--_finish;
		destroy(_finish);
	}

	void MakeHeap(Iterator first, Iterator last)
	{
		if (last - first < 2)return;
		unsigned len = last - first;
		unsigned parent = len / 2;
		while (true)
		{
			adjustHeap(first, parent, len, _Ty(*(first + parent)));
			if (parent == 0)return;
			--parent;
		}
	}

	void PushHeap(Iterator first, Iterator last)
	{
		unsigned holeIndex = last - first - 1;
		unsigned topIndex = 0;
		_Ty value = _Ty(*(last - 1));
		pushHeap(first, holeIndex, topIndex, value);
	}

	void PopHeap(Iterator first, Iterator last)
	{
		popHeap(first, last - 1, last - 1, T(*(last - 1)));
	}

	void SortHeap(Iterator first, Iterator last)
	{
		while (last - first > 1)
		{
			PopHeap(first, last--);
		}
	}

protected:
	void insertAux(Iterator pos, const _Ty& value)
	{
		if (_finish != _endOfStorage)
		{
			construct(_finish, *(_finish - 1));
			++_finish;
			_Ty x_copy = value;
			std::copy_backward(pos, _finish - 2, _finish - 1);
			*pos = x_copy;
		}
		else
		{
			unsigned old_size = Size();
			const unsigned len = old_size != 0 ? 2 * old_size : 1;
			Iterator new_start = _alloctor.Allocate(len);
			Iterator new_finish = new_start;
			try
			{
				new_finish = std::uninitialized_copy(_start, pos, new_start);
				construct(new_finish, value); 
				++new_finish;
			}
			catch (std::exception ex)
			{
				destroy(new_start, new_finish);
				_alloctor.Deallocate(_start);
				throw;
			}
			destroy(Begin(), End());
			deallocate();
			_start = new_start;
			_finish = new_finish;
			_endOfStorage = new_start + len;
		}
	}

	void popHeap(Iterator first, Iterator last, Iterator result, _Ty value)
	{
		*result = *first;
		adjustHeap(first, unsigned(0), unsigned(last - first), value);
	}

	void adjustHeap(Iterator first, unsigned holeIndex, unsigned len, _Ty value)
	{
		unsigned topIndex = holeIndex;
		unsigned secondChild = 2 * holeIndex + 2;
		while (secondChild < len)
		{
			if (*(first + secondChild) < *(first + (secondChild - 1)))
			{
				--secondChild;
			}
			*(first + holeIndex) = *(first + secondChild);
			holeIndex = secondChild;
			secondChild = 2 * (secondChild + 1);
		}
		if (secondChild == len)
		{
			*(first + holeIndex) = *(first + (secondChild - 1));
			holeIndex = secondChild - 1;
		}
		pushHeap(first, holeIndex, topIndex, value);
	}

	void pushHeap(Iterator first, unsigned holeIndex, unsigned topIndex, _Ty value)
	{
		unsigned parent = (holeIndex - 1) / 2;
		while (holeIndex > topIndex && *(first + parent) < value)
		{
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}
	
protected:
	void fillInitialize(unsigned size, const _Ty& value)
	{
		_start = allocateAndFill(size, value);
		_finish = _start + size;
		_endOfStorage = _finish;
	}

	Iterator allocateAndFill(unsigned size, const _Ty& x)
	{
		Iterator result = _alloctor.Allocate(size);
		Iterator cur = result;
		for (; size > 0; --size, ++cur)
		{
			construct(&*cur, x);
		}
		return result;
	}
	void deallocate()
	{
		if (_start)
		{
			_alloctor.Deallocate(_start);
		}
	}
private:

	template<class _Ty1, class  _Ty2>
	inline void construct(_Ty1* pointer, const _Ty2& value)
	{
		new (pointer)_Ty1(value);
	}

	template<typename T>
	inline void destroy(T* pointer)
	{
		pointer->~T();
	}

	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last)
	{
		for (; first < last; ++first)
			destroy(&*first);
	}
private:
	Iterator _start;
	Iterator _finish;
	Iterator _endOfStorage;
	_Alloc _alloctor;
};

}

#endif // !KH_STL_TYPE_HEAP_H_