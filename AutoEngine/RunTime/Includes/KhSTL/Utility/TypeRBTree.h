#ifndef KH_STL_TYPE_RED_BLACK_TREE_H_
#define KH_STL_TYPE_RED_BLACK_TREE_H_

#include "../Allocator/TypeAllocator.h"
#include "../Utility/TypeDef.h"
#include "../Utility/TypeTreeNode.h"
#include "../Utility/TypeRBTreeIterator.h"
#include "../Utility/TypePair.h"
#include "TypeRBTreeAlloc.h"

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

namespace KhSTL{

template<typename _Traits
	,typename _Comp
	,typename _Alloc>
	class RBTree : public tRBTreeAlloc<_Traits, _Comp, _Alloc>
{
protected:
	using KeyType = typename _Traits::KeyType;

	using ValueType = typename _Traits::ValueType;

	using Compare = _Comp;

	using ColorType = RBTreeColorType; 

	using Base = tRBTreeAlloc<_Traits, _Comp, _Alloc>;

	using Alloc = tRBTreeAlloc<_Traits, _Comp, _Alloc>;

	using Iterator = tRBTreeIterator<_Traits>;

	using ConstIterator = const tRBTreeIterator<_Traits>;

	using NodeType = typename Alloc::NodeType;
public:
	/**
	* @brief : Constructed
	*/
	RBTree() 
		: _count(0)
		, _comp(Compare())
	{ 
		_header = Alloc::getNode();
		getColor(_header) = RBTreeColorType::RED;
		root() = 0;
		leftmost() = _header;
		rightmost() = _header;
	}
	/**
	* @brief : Destructing
	*/
	~RBTree()
	{
		Alloc::putNode(_header);
	}

protected:
	/**
	* @brief : Get the root node
	*/
	NodeType*& root()const 
	{ 
		return (NodeType*&)_header->parent; 
	}
	/**
	* @brief : Get the leftmost node
	*/
	NodeType*& leftmost()const 
	{ 
		return (NodeType*&)_header->left; 
	}
	/**
	* @brief : Get the rightmost node
	*/
	NodeType*& rightmost()const 
	{ 
		return (NodeType*&)_header->right; 
	}
	/**
	* @brief : Returns the left child node of the node
	*/
	NodeType*& left(NodeType* node)
	{ 
		return (NodeType*&)(node->left);
	}
	/**
	* @brief : Returns the right child node of the node
	*/
	NodeType*& right(NodeType* node)
	{ 
		return (NodeType*&)(node->right);
	}
	/**
	* @brief : Returns the parent node of the node
	*/
	NodeType*& parent(NodeType* node)
	{ 
		return (NodeType*&)(node->parent);
	}
	KeyType& getKey(NodeType* node)
	{
		return node->key;
	}
	/**
	* @brief : Returns the value of the node
	*/
	ValueType& getValue(NodeType* node)
	{
		return node->value;
	}

	/**
	* @brief : Returns the color of the node
	*/
	ColorType& getColor(NodeType* node)
	{ 
		return (ColorType&)(node->color);
	}

	/**
	* @brief : Return minimum node
	*/
	NodeType* minimum(NodeType* node)
	{
		return (NodeType*)NodeType::Minimum(node);
	}
	/**
	* @brief : Return maximum node
	*/
	NodeType* maximum(NodeType* node)
	{
		return (NodeType*)NodeType::Maximum(node);
	}

public:
	/**
	* @brief : Get the value of the root node
	*/
	ValueType RootValue()
	{ 
		return getValue((NodeType*)_header->parent); 
	}
	/**
	* @brief : Return a function that compares the size
	*/
	Compare KeyComp()const
	{
		return _comp;
	}
	/**
	* @brief : Return leftmost iterator
	*/
	inline Iterator Begin() { return leftmost(); }
	/**
	* @brief : Return leftmost const iterator
	*/
	inline ConstIterator Begin() const { return leftmost(); }
	/**
	* @brief : Return head iterator
	*/
	inline Iterator End() { return _header; }
	/**
	* @brief : Return head const iterator
	*/
	inline ConstIterator End() const { return _header; }
	/**
	* @brief : To determine if the red-black tree is Empty
	*/
	bool Empty()const 
	{ 
		return _count == 0; 
	}
	/**
	* @brief : Retrurn tree count
	*/
	unsigned GetSize() const 
	{ 
		return _count; 
	}
	/**
	* @brief : Insert new value, node key value cannot be repeated, 
	*			insert invalid if repeated
	*/
	Iterator InsertUnique(const KeyType& key,const ValueType& value)
	{
		NodeType* pHeader = _header; 
		NodeType* pRoot = root(); 
		bool comp = true; 

		while (pRoot != 0) 
		{
			pHeader = pRoot; 
			comp = _comp(key, getKey(pRoot));
			pRoot = comp ? left(pRoot) : right(pRoot);
		}
		Iterator tmp = Iterator(pHeader);
		if (comp)
		{
			if (Begin() == tmp)
			{
				return Iterator(insertNode(pRoot, pHeader, key ,value));
			}
			else
			{
				tmp--;
			}
		}
		if (_comp(getKey(tmp.node), key))
		{
			return Iterator(insertNode(pRoot, pHeader, key, value));
		}
		return Iterator(tmp);
	}
	/**
	* @brief : Insert a new value, the node's key value allows repetition
	*/
	Iterator InsertEqual(const KeyType& key, const ValueType& val)
	{
		NodeType* pHeader = _header;
		NodeType* pRoot = root();
		while (pRoot != 0)
		{
			pHeader = pRoot;
			pRoot = _comp(key, getKey(pRoot)) ? left(pRoot) : right(pRoot);
		}
		return insertNode(pRoot, pHeader, key, val);
	}
	/**
	* @brief : Look for a node in a red-black tree with a key value of key
	*/
	Iterator Find(const KeyType& key)
	{
		NodeType* pHeader = _header; 
		NodeType* pRoot = root(); 
		while (pRoot != 0)
		{
			if (!_comp(getKey(pRoot), key))
			{
				pHeader = pRoot;
				pRoot = left(pRoot);
			}
			else
			{
				pRoot = right(pRoot);
			}
		}
		Iterator tmp = Iterator(pHeader);
		return (tmp == End() || _comp(key, getKey(tmp.node))) ? End() : tmp;
	}

private:
	/**
	* @brief : Insert node
	* @param : insertWith : To insert node 
	*		 : insertTarget : Insert the parent node of the node
	*/
	Iterator insertNode(NodeType* insertWith, NodeType* insertTarget, const KeyType& key , const ValueType& val)
	{
		NodeType* tmpWith = (NodeType*)insertWith; 
		NodeType* tmpTarget = (NodeType*)insertTarget; 
		NodeType* tmp;

		if (tmpTarget == _header || tmpWith != 0 || _comp(key, getKey(tmpTarget)))
		{
			tmp = Alloc::getNode(key,val);
			left(tmpTarget) = tmp; 
			if (tmpTarget == _header) 
			{
				root() = tmp;
				rightmost() = tmp;
			}
			else if (tmpTarget == leftmost())
			{
				leftmost() = tmp;
			}
		}
		else
		{
			tmp = Alloc::getNode(key,val);
			right(tmpTarget) = tmp;
			if (tmpTarget == rightmost())
			{
				rightmost() = tmp;
			}
		}
		parent(tmp) = tmpTarget;
		left(tmp) = 0;
		right(tmp) = 0;

		RBTreeRebalance(tmp, _header->parent);
		++_count;
		return Iterator(tmp);
	}
	/**
	* @brief : Rebalance the red-black tree
	*/
	void RBTreeRebalance(NodeType* val, NodeType*& root)
	{
		val->color = RBTreeColorType::RED;
		while (val != root && val->parent->color == RBTreeColorType::RED)
		{
			if (val->parent == val->parent->parent->left)
			{
				NodeType* tmp = val->parent->parent->right;
				if (tmp && tmp->color == RBTreeColorType::RED)
				{
					val->parent->color = RBTreeColorType::BLACK;
					tmp->color = RBTreeColorType::BLACK;
					val->parent->parent->color = RBTreeColorType::RED;
					val = val->parent->parent;
				}
				else
				{
					if (val == val->parent->right)
					{
						val = val->parent;
						RBTreeRotateLeft(val, root);
					}
					val->parent->color = RBTreeColorType::BLACK;
					val->parent->parent->color = RBTreeColorType::RED;
					RBTreeRotateRight(val->parent->parent, root);
				}
			}
			else 
			{
				NodeType* tmp = val->parent->parent->left;
				if (tmp && tmp->color == RBTreeColorType::RED)
				{
					val->parent->color = RBTreeColorType::BLACK;
					tmp->color = RBTreeColorType::BLACK;
					val->parent->parent->color = RBTreeColorType::RED;
					val = val->parent->parent;
				}
				else
				{
					
					if (val == val->parent->left)
					{
						val = val->parent;
						RBTreeRotateRight(val, root);
					}
					val->parent->color = RBTreeColorType::BLACK;
					val->parent->parent->color = RBTreeColorType::RED;
					RBTreeRotateLeft(val->parent->parent, root); 
				}
			}
		}
		root->color = RBTreeColorType::BLACK;
	}
	/**
	* @brief : Light handed rotation 
	*/
	void RBTreeRotateLeft(NodeType* val, NodeType*& root)
	{
		NodeType* pRight = val->right;
		val->right = pRight->left;
		if (pRight->left != 0)
		{
			pRight->left->parent = val;
		}
		pRight->parent = val->parent;

		if (val == root)
		{
			root = pRight;
		}
		else if (val == val->parent->left)
		{
			val->parent->left = pRight;
		}
		else
		{
			val->parent->right = pRight;
		}
		pRight->left = val;
		val->parent = pRight;
	}
	/**
	* @brief : Right handed rotation 
	*/
	void RBTreeRotateRight(NodeType* val, NodeType*& root)
	{
		NodeType* pLeft = val->left;
		val->left = pLeft->right;
		if (pLeft->right != 0)
		{
			pLeft->right->parent = val;
		}
		pLeft->parent = val->parent;
		if (val == root)
		{
			root = pLeft;
		}
		else if (val == val->parent->right)
		{
			val->parent->right = pLeft;
		}
		else
		{
			val->parent->left = pLeft;
		}
		pLeft->right = val;
		val->parent = pLeft;
	}

protected:
	/// Red black tree count
	unsigned _count;
	NodeType* _header;
	Compare _comp;

};

}

#endif // !KH_STL_TYPE_RED_BLACK_TREE_H_
