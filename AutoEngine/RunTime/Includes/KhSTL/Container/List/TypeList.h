#ifndef KH_STL_TYPE_LIST_H_
#define KH_STL_TYPE_LIST_H_

#include "TypeListAllocator.h"

#include "TypeListIterator.h"

#include <initializer_list>
#include "../../Utility/TypeReverseIterator.h"
namespace KhSTL {


/**
* Doubly-linked list template class
*/
template <typename _Ty,
	typename _Alloc = tAllocator<tListNode<_Ty>>>
	class tList : public tListAlloc <_Ty, _Alloc>
{
public:
	/// Value type
	using ValueType = _Ty;
	/// This class
	using This = tList<_Ty, _Alloc>;
	/// Base class
	using Base = tListAlloc <_Ty, _Alloc>;
	/// Allocator
	using Alloc = tListAlloc <_Ty, _Alloc>;
	/// Value info
	using Value = typename Alloc::Value;
	/// Allocator type
	using AllocatorType = typename _Alloc::ValueType;
	/// List iterator
	using Iterator = tListIterator<_Ty>;
	/// List const iterator
	using ConstIterator = tConstListIterator<_Ty>;
	/// List reverse iterator
	using ReverseIterator = tReverseIterator<Iterator>;
	/// List const reverse iterator
	using ConstReverseIterator = tReverseIterator<ConstIterator>;
public:
	/**
	* @brief : Construct Empty
	*/
	tList()
		: Base()
	{}
	/**
	* @brief : Construct from another list
	*/
	tList(const tList<_Ty>& list)
		: Base(list.Size() + 1)
	{
		*this = list;
	}
	/**
	* @brief : Move-construct from another list
	*/
	tList(tList<_Ty> && list) noexcept
	{
		Swap(list);
	}
	/**
	* @brief : Aggregate initialization constructor
	*/
	tList(const std::initializer_list<_Ty>& list) 
		: tList()
	{
		for (auto it = list.begin(); it != list.end(); it++)
		{
			PushBack(*it);
		}
	}
	/**
	* @brief : Destruct
	*/
	~tList()
	{
		Clear();
	}
	/**
	* @brief : Assign from another list
	*/
	tList& operator =(const tList<_Ty>& rhs)
	{
		// Clear, then insert the nodes of the other list. In case of This-assignment do nothing
		if (&rhs != this)
		{
			Clear();
			Insert(End(), rhs);
		}
		return *this;
	}
	/**
	* @brief : Move-assign from another list
	*/
	tList& operator =(tList<_Ty> && rhs) noexcept
	{
		assert(&rhs != this);
		Swap(rhs);
		return *this;
	}
	/**
	* @brief : Add-assign an element
	*/
	tList& operator +=(const _Ty& rhs)
	{
		Push(rhs);
		return *this;
	}
	/**
	* @brief : Add-assign a list
	*/
	tList& operator +=(const tList<_Ty>& rhs)
	{
		Insert(End(), rhs);
		return *this;
	}
	/**
	* @brief : Test for equality with another list
	*/
	bool operator ==(const tList<_Ty>& rhs) const
	{
		if (rhs._size != Base::_size)
			return false;

		ConstIterator i = Begin();
		ConstIterator j = rhs.Begin();
		while (i != End())
		{
			if (*i != *j)
				return false;
			++i;
			++j;
		}

		return true;
	}
	/**
	* @brief : Test for inequality with another list
	*/
	bool operator !=(const tList<_Ty>& rhs) const
	{
		if (rhs._size != Base::_size)
			return true;

		ConstIterator i = Begin();
		ConstIterator j = rhs.Begin();
		while (i != End())
		{
			if (*i != *j)
				return true;
			++i;
			++j;
		}

		return false;
	}
	/**
	* @brief : Insert an element to the end
	*/
	void PushBack(const _Ty& value) { Base::insertNode(Base::tail(), value); }
	/**
	* @brief : Insert an element to the beginning
	*/
	void PushFront(const _Ty& value) { Base::insertNode(Base::head(), value); }
	/**
	* @brief : Insert an element at position
	*/
	void Insert(const Iterator& dest, const _Ty& value) { insertNode(static_cast<tListNode<_Ty>*>(dest.ptr), value); }
	/**
	* @brief : Insert a list at position
	*/
	void Insert(const Iterator& dest, const tList<_Ty>& list)
	{
		auto* destNode = static_cast<tListNode<_Ty>*>(dest.ptr);
		ConstIterator it = list.Begin();
		ConstIterator end = list.End();
		while (it != end)
			insertNode(destNode, *it++);
	}
	/**
	* @brief : Insert elements by iterators
	*/
	void Insert(const Iterator& dest, const ConstIterator& start, const ConstIterator& end)
	{
		auto* destNode = static_cast<tListNode<_Ty>*>(dest.ptr);
		ConstIterator it = start;
		while (it != end)
			insertNode(destNode, *it++);
	}
	/**
	* @brief : Insert elements
	*/
	void Insert(const Iterator& dest, const _Ty* start, const _Ty* end)
	{
		auto* destNode = static_cast<tListNode<_Ty>*>(dest.ptr);
		const _Ty* ptr = start;
		while (ptr != end)
			insertNode(destNode, *ptr++);
	}
	/**
	* @brief : Erase the last element
	*/
	void PopBack()
	{
		if (Base::_size)
			Erase(--End());
	}
	/**
	* @brief : Erase the first element
	*/
	void PopFront()
	{
		if (Base::_size)
			Erase(Begin());
	}
	/**
	* @brief : Erase an element by iterator. Return iterator to the next element
	*/
	Iterator Erase(Iterator it)
	{
		return Iterator(eraseNode(static_cast<tListNode<_Ty>*>(it.ptr)));
	}
	/**
	* @brief : Erase a range by iterators. Return an iterator to the next element
	*/
	Iterator Erase(const Iterator& start, const Iterator& end)
	{
		Iterator it = start;
		while (it != end)
			it = Erase(it);

		return it;
	}
	/**
	* @brief : Erase each element matching value
	*/
	void Remove(const _Ty& value)
	{
		Iterator it = End();

		for (Iterator firstIt = Begin(); firstIt != End(); /**/)
			if (*firstIt == value)
				if (std::addressof(*firstIt) == std::addressof(value))
					it = firstIt++;
				else
					firstIt = Erase(firstIt);
			else
				++firstIt;

		if (it != End())
			Erase(it);
	}

	/**
	* @brief : Clear the list
	*/
	void Clear()
	{
		if (Size())
		{
			for (Iterator i = Begin(); i != End();)
			{
				Base::freeNode(static_cast<tListNode<_Ty>*>(i++.ptr));
				i.ptr->prev = 0;
			}

			Base::_head = Base::_tail;
			Base::_size = 0;
		}
	}
	/**
	* @brief : Resize the list by removing or adding items at the end
	*/
	void Resize(unsigned newSize)
	{
		while (Base::_size > newSize)
			PopBack();

		while (Base::_size < newSize)
			insertNode(Base::tail(), _Ty());
	}
	/**
	* @brief : Return iterator to value, or to the end if not found
	*/
	Iterator Find(const _Ty& value)
	{
		Iterator it = Begin();
		while (it != End() && *it != value)
			++it;
		return it;
	}
	/**
	* @brief : Return const iterator to value, or to the end if not found
	*/
	ConstIterator Find(const _Ty& value) const
	{
		ConstIterator it = Begin();
		while (it != End() && *it != value)
			++it;
		return it;
	}
	/**
	* @brief : Return whether contains a specific value
	*/
	bool Contains(const _Ty& value) const { return Find(value) != End(); }
	/**
	* @brief : Return iterator to the first element
	*/
	inline Iterator Begin() { return Iterator(Base::head()); }
	/**
	* @brief : Return iterator to the first element
	*/
	inline ConstIterator Begin() const { return ConstIterator(Base::head()); }

	/**
	* @brief : Return iterator to the end
	*/
	inline Iterator End() { return Iterator(Base::tail()); }
	/**
	* @brief : Return iterator to the end
	*/
	inline ConstIterator End() const { return ConstIterator(Base::tail()); }

	inline ReverseIterator RBegin() { return ReverseIterator(End()); }

	inline ConstReverseIterator RBegin() const { return ReverseIterator(End()); }

	inline ReverseIterator REnd() { return ReverseIterator(Begin()); }

	inline ConstReverseIterator REnd() const { return ReverseIterator(Begin()); }

	/**
	* @brief : Return first element
	*/
	_Ty& Front() { return *Begin(); }
	/**
	* @brief : Return const first element
	*/
	const _Ty& Front() const { return *Begin(); }
	/**
	* @brief : Return last element
	*/
	_Ty& Back() { return *(--End()); }
	/**
	* @brief : Return const last element
	*/
	const _Ty& Back() const { return *(--End()); }
	/**
	* @brief : Return number of elements
	*/
	unsigned Size() const { return Base::_size; }
	/**
	* @brief : Return whether list is Empty
	*/
	bool Empty() const { return Base::_size == 0; }

};
	
template <typename _Ty> typename tList<_Ty>::ConstIterator begin(const tList<_Ty>& v) { return v.Begin(); }

template <typename _Ty> typename tList<_Ty>::ConstIterator end(const tList<_Ty>& v) { return v.End(); }

template <typename _Ty> typename tList<_Ty>::Iterator begin(tList<_Ty>& v) { return v.Begin(); }

template <typename _Ty> typename tList<_Ty>::Iterator end(tList<_Ty>& v) { return v.End(); }

template <typename _Ty> typename tList<_Ty>::ConstIterator rbegin(const tList<_Ty>& v) { return v.End(); }

template <typename _Ty> typename tList<_Ty>::ConstIterator rend(const tList<_Ty>& v) { return v.Begin(); }

template <typename _Ty> typename tList<_Ty>::Iterator rbegin(tList<_Ty>& v) { return v.End(); }

template <typename _Ty> typename tList<_Ty>::Iterator rend(tList<_Ty>& v) { return v.Begin(); }

}
#endif //!KH_STL_TYPE_LIST_H_
