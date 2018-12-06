#ifndef KH_STL_LIST_ITERATOR_H_
#define KH_STL_LIST_ITERATOR_H_

#include "TypeListValue.h"

namespace KhSTL {

/**
* Doubly-linked list iterator Base class
*/
struct tListIteratorBase
{
	/**
	* @brief : Construct
	*/
	tListIteratorBase()
		: ptr(nullptr)
	{}
	/**
	* @brief : Construct with a node pointer
	*/
	explicit tListIteratorBase(tListNodeBase* ptr)
		: ptr(ptr)
	{}
	/**
	* @brief : Test for equality with another iterator
	*/
	bool operator ==(const tListIteratorBase& rhs) const { return ptr == rhs.ptr; }
	/**
	* @brief : Test for inequality with another iterator
	*/
	bool operator !=(const tListIteratorBase& rhs) const { return ptr != rhs.ptr; }
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

	/// Node pointer.
	tListNodeBase* ptr;
};

/**
	* List iterator
	*/
template <typename _Ty>
	struct tListIterator 
	: public tListIteratorBase
{
	using ValueType = _Ty;
	/**
	* @brief : Construct
	*/
	tListIterator() = default;
	/**
	* @brief : Construct with a node pointer
	*/
	explicit tListIterator(tListNode<_Ty>* ptr)
		: tListIteratorBase(ptr)
	{}
	/**
	* @brief : Preincrement the pointer
	*/
	tListIterator& operator ++()
	{
		GotoNext();
		return *this;
	}
	/**
	* @brief : Postincrement the pointer
	*/
	tListIterator operator ++(int)
	{
		tListIterator it = *this;
		GotoNext();
		return it;
	}
	/**
	* @brief : Predecrement the pointer
	*/
	tListIterator& operator --()
	{
		GotoPrev();
		return *this;
	}
	/**
	* @brief : Postdecrement the pointer
	*/
	tListIterator operator --(int)
	{
		tListIterator it = *this;
		GotoPrev();
		return it;
	}
	/**
	* @brief : Point to the node value
	*/
	_Ty* operator ->() const { return &(static_cast<tListNode<_Ty>*>(ptr))->value; }
	/**
	* @brief : Dereference the node value
	*/
	_Ty& operator *() const { return (static_cast<tListNode<_Ty>*>(ptr))->value; }
};

/**
* List const iterator
*/
template <typename _Ty>
	struct tConstListIterator 
	: public tListIteratorBase
{
	using ValueType = _Ty;
	/**
	* @brief : Construct
	*/
	tConstListIterator() = default;
	/**
	* @brief : Construct with a node pointer
	*/
	explicit tConstListIterator(tListNode<_Ty>* ptr)
		: tListIteratorBase(ptr)
	{}
	/**
	* @brief : Construct from a non-const iterator
	*/
	tConstListIterator(const tListIterator<_Ty>& rhs)
		: tListIteratorBase(rhs.ptr)
	{}
	/**
	* @brief : Assign from a non-const iterator
	*/
	tConstListIterator& operator =(const tListIterator<_Ty>& rhs)
	{
		ptr = rhs.ptr;
		return *this;
	}
	/**
	* @brief : Preincrement the pointer
	*/
	tConstListIterator& operator ++()
	{
		GotoNext();
		return *this;
	}
	/**
	* @brief : Postincrement the pointer
	*/
	tConstListIterator operator ++(int)
	{
		tConstListIterator it = *this;
		GotoNext();
		return it;
	}
	/**
	* @brief : Predecrement the pointer
	*/
	tConstListIterator& operator --()
	{
		GotoPrev();
		return *this;
	}
	/**
	* @brief : Postdecrement the pointer
	*/
	tConstListIterator operator --(int)
	{
		tConstListIterator it = *this;
		GotoPrev();
		return it;
	}
	/**
	* @brief : Point to the node value
	*/
	const _Ty* operator ->() const { return &(static_cast<tListNode<_Ty>*>(ptr))->value; }
	/**
	* @brief : Dereference the node value
	*/
	const _Ty& operator *() const { return (static_cast<tListNode<_Ty>*>(ptr))->value; }
};

}
#endif //!KH_STL_LIST_ITERATOR_H_s