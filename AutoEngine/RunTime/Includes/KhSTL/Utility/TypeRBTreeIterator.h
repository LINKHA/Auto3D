#ifndef KH_STL_TYPE_TREE_ITERATOR_H_
#define KH_STL_TYPE_TREE_ITERATOR_H_

#include "TypeTreeNode.h"
#include <iterator>

namespace KhSTL {

template<typename _Traits>
class tRBTreeBaseIterator
{
public:
	tRBTreeNode<_Traits>* node;

	/**
	* @brief : This function is called when subclasses of the iterator implement operator
	*/
	void Increment()
	{
		if (node->right != 0)
		{
			node = node->right;
			while (node->left != 0)
			{
				node = node->left;
			}
		}
		else
		{
			tRBTreeNode<_Traits>* y = node->parent;
			while (node == y->right)
			{
				node = y;
				y = y->parent;
			}
			if (node->right != y)
			{
				node = y;
			}
		}
	}
	/**
	* @brief : This function is called when subclasses of the iterator implement operator --
	*/
	void Decrement()
	{
		if (node->color == RBTreeColorType::RED && node->parent->parent == node)
		{
			node = node->right;
		}
		else if (node->left != 0)
		{
			tRBTreeNode<_Traits>* y = node->left;
			while (y->right != 0)
			{
				y = y->right;
			}
			node = y;
		}
		else
		{
			tRBTreeNode<_Traits>* y = node->parent;
			while (node == y->left)
			{
				node = y;
				y = y->parent;
			}
			node = y;
		}
	}
};

template<typename _Traits>
class tRBTreeIterator
	: public tRBTreeBaseIterator<_Traits>
{
public:
	using ValueType = typename _Traits::ValueType;

	using Base = tRBTreeBaseIterator<_Traits>;

	using Iterator = tRBTreeIterator<_Traits>;

	using This = tRBTreeIterator<_Traits>;

	using LinkType = tRBTreeNode<_Traits>*;

	tRBTreeIterator() = default;

	tRBTreeIterator(LinkType x)
	{
		Base::node = x;
	}
	tRBTreeIterator(const Iterator& it)
	{
		Base::node = it.node;
	}
	/**
	* @brief : Dereference, return node value
	*/
	ValueType& operator *() const
	{
		return LinkType(Base::node)->value;
	}
	/**
	* @brief : Dereference, return node value
	*/
	ValueType* operator ->() const
	{
		return &(operator *());
	}
	/**
	* @brief : Returns the color of the node the iterator points to
	*/
	RBTreeColorType Color()
	{
		return Base::node->color;
	}
	/**
	* @brief : Iterator stepIterator step
	*/
	This& operator ++()
	{
		Base::Increment();
		return *this;
	}
	/**
	* @brief : Iterator step
	*/
	This& operator ++(int)
	{
		This tmp = *this;
		Base::Increment();
		return tmp;
	}
	/**
	* @brief : Iterator step
	*/
	This& operator --() const
	{
		Base::Decrement();
		return *this;
	}
	/**
	* @brief : Iterator step
	*/
	This& operator --(int)
	{
		This tmp = *this;
		Base::Decrement();
		return tmp;
	}
	/**
	* @brief : Comparison of two iterators
	*/
	bool operator ==(const This& rhs) const
	{
		return rhs.node == Base::node;
	}
	/**
	* @brief : Comparison of two iterators
	*/
	bool operator !=(const This& rhs) const
	{
		return rhs.node != Base::node;
	}
};



}

#endif //!KH_STL_TYPE_TREE_ITERATOR_H_