#ifndef KH_STL_TYPE_HASH_ITERATOR_H_
#define KH_STL_TYPE_HASH_ITERATOR_H_

#include "TypeHashValue.h"

namespace KhSTL {

/**
* Hash set/map iterator Base class
*/
class tHashIteratorBase
{
public:
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
/**
* Hash map node iterator
*/
template <typename _Ty>
class tHashIterator : public tHashIteratorBase
{
public:
	using ValueType = _Ty;
public:
	/**
	* @brief : Construct
	*/
	tHashIterator() = default;
	/**
	* @brief : Construct with a node pointer
	*/
	explicit tHashIterator(tHashNode<_Ty>* ptr) :
		tHashIteratorBase(ptr)
	{
	}
	/**
	* @brief : Postincrement the pointer
	*/
	tHashIterator operator +(int offset)
	{
		int count = std::abs(offset);
		tHashIterator it = *this;
		while (count--)
			offset > 0 ? GotoNext() : GotoPrev();
		return it;
	}
	/**
	* @brief : Preincrement the pointer
	*/
	tHashIterator& operator ++()
	{
		GotoNext();
		return *this;
	}
	/**
	* @brief : Postincrement the pointer
	*/
	tHashIterator operator ++(int)
	{
		tHashIterator it = *this;
		GotoNext();
		return it;
	}
	/**
	* @brief : Postincrement the pointer
	*/
	tHashIterator operator -(int offset)
	{
		int count = std::abs(offset);
		tHashIterator it = *this;
		while (count--)
			offset < 0 ? GotoNext() : GotoPrev();
		return it;
	}
	/**
	* @brief : Predecrement the pointer
	*/
	tHashIterator& operator --()
	{
		GotoPrev();
		return *this;
	}
	/**
	* @brief : Postdecrement the pointer
	*/
	tHashIterator operator --(int)
	{
		tHashIterator it = *this;
		GotoPrev();
		return it;
	}
	/**
	* @brief : Point to the pair
	*/
	_Ty* operator ->() const { return &(static_cast<tHashNode<_Ty>*>(ptr))->data; }
	/**
	* @brief : Dereference the pair
	*/
	_Ty& operator *() const { return (static_cast<tHashNode<_Ty>*>(ptr))->data; }
};
/**
* Hash map node const iterator
*/
template <typename _Ty>
class tConstHashIterator : public tHashIteratorBase
{
public:
	/**
	* @brief : Construct
	*/
	tConstHashIterator() = default;
	/**
	* @brief : Construct with a node pointer
	*/
	explicit tConstHashIterator(tHashNode<_Ty>* ptr) :
		tHashIteratorBase(ptr)
	{
	}
	/**
	* @brief : Construct from a non-const iterator
	*/
	tConstHashIterator(const tHashIterator<_Ty>& rhs) :        // NOLIN_Key(google-explicit-constructor)
		tHashIteratorBase(rhs.ptr)
	{}
	/**
	* @brief : Assign from a non-const iterator
	*/
	tConstHashIterator& operator =(const tHashIterator<_Ty>& rhs)
	{
		ptr = rhs.ptr;
		return *this;
	}
	/**
	* @brief : Preincrement the pointer
	*/
	tConstHashIterator& operator ++()
	{
		GotoNext();
		return *this;
	}
	/**
	* @brief : Postincrement the pointer
	*/
	tConstHashIterator operator ++(int)
	{
		tConstHashIterator it = *this;
		GotoNext();
		return it;
	}
	/**
	* @brief : Predecrement the pointer
	*/
	tConstHashIterator& operator --()
	{
		GotoPrev();
		return *this;
	}
	/**
	* @brief : Postdecrement the pointer
	*/
	tConstHashIterator operator --(int)
	{
		tConstHashIterator it = *this;
		GotoPrev();
		return it;
	}
	/**
	* @brief : Point to the pair
	*/
	const _Ty* operator ->() const { return &(static_cast<tHashNode<_Ty>*>(ptr))->data; }
	/**
	* @brief : Dereference the pair
	*/
	const _Ty& operator *() const { return (static_cast<tHashNode<_Ty>*>(ptr))->data; }
};

}

#endif // KH_STL_TYPE_HASH_ITERATOR_H_
