#ifndef KH_STL_TYPE_TREE_ITERATOR_H_
#define KH_STL_TYPE_TREE_ITERATOR_H_

#include "TypeTreeNode.h"
#include <iterator>

namespace KhSTL {

template<typename _Traits>
class tRBTreeIterator
{
public:
	using ValueType = typename _Traits::ValueType;

	using Iterator = tRBTreeIterator<_Traits>;

	using This = tRBTreeIterator<_Traits>;

	using LinkType = tRBTreeNode<_Traits>*;
public:
	tRBTreeIterator() = default;

	tRBTreeIterator(LinkType x)
	{
		node = x;
	}
	tRBTreeIterator(const Iterator& it)
	{
		node = it.node;
	}
	/**
	* @brief : Dereference, return node value
	*/
	ValueType& operator *() const
	{
		return LinkType(node)->value;
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
		return node->color;
	}
	/**
	* @brief : Iterator stepIterator step
	*/
	This operator ++()
	{
		Increment();
		return *this;
	}
	/**
	* @brief : Iterator step
	*/
	This& operator ++(int)
	{
		This tmp = *this;
		Increment();
		return tmp;
	}
	/**
	* @brief : Iterator step
	*/
	This operator --() const
	{
		if (node->color == RBTreeColorType::RED && node->parent->parent == node)
		{
			node = node->right;
		}
		else if (node->left != 0)
		{
			tRBTreeNode<_Traits>* tmp = node->left;
			while (tmp->right != 0)
			{
				tmp = tmp->right;
			}
			this->node = tmp;
		}
		else
		{
			tRBTreeNode<_Traits>* tmp = node->parent;
			while (node == tmp->left)
			{
				node = tmp;
				tmp = tmp->parent;
			}
			node = tmp;
		}

		return *this;
	}
	/**
	* @brief : Iterator step
	*/
	This& operator --(int)
	{
		This tmp = *this;
		Decrement();
		return tmp;
	}
	/**
	* @brief : Comparison of two iterators
	*/
	bool operator ==(const This& rhs) const
	{
		return rhs.node == node;
	}
	/**
	* @brief : Comparison of two iterators
	*/
	bool operator !=(const This& rhs) const
	{
		return rhs.node != node;
	}
	
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
			tRBTreeNode<_Traits>* tmp = node->parent;
			while (node == tmp->right)
			{
				node = tmp;
				tmp = tmp->parent;
			}
			if (node->right != tmp)
			{
				node = tmp;
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
			tRBTreeNode<_Traits>* tmp = node->left;
			while (tmp->right != 0)
			{
				tmp = tmp->right;
			}
			node = tmp;
		}
		else
		{
			tRBTreeNode<_Traits>* tmp = node->parent;
			while (node == tmp->left)
			{
				node = tmp;
				tmp = tmp->parent;
			}
			node = tmp;
		}
	}
	tRBTreeNode<_Traits>* node;
};



}

#endif //!KH_STL_TYPE_TREE_ITERATOR_H_