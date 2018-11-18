#ifndef KH_STL_TYPE_TREE_NODE_H_
#define KH_STL_TYPE_TREE_NODE_H_

#include "../Allocator/TypeAllocator.h"
#include "../Utility/TypeDef.h"

namespace KhSTL {

enum class RBTreeColorType
{
	RED,
	BLACK
};

template<typename _Traits>
class tRBTreeNode
{
public:
	using KeyType = typename _Traits::KeyType;

	using ValueType = typename _Traits::ValueType;

	using ColorType = RBTreeColorType;

	using LinkType = tRBTreeNode<_Traits>*;

public:
	tRBTreeNode() = default;

	tRBTreeNode(const KeyType& skey, const ValueType& svalue)
		: key(skey)
		, value(svalue)
	{}
	~tRBTreeNode() = default;
	/**
	* @brief : Find most left node
	*/
	tRBTreeNode* Minimum(tRBTreeNode* x)
	{
		while (x->left != 0)
		{
			x = x->left;
		}
		return x;
	}
	/**
	* @brief : Find most right node
	*/
	tRBTreeNode* Maximum(tRBTreeNode* x)
	{
		while (x->right != 0)
		{
			x = x->right;
		}
		return x;
	}
	/// Node color
	ColorType color;
	/// Node parent
	tRBTreeNode<_Traits>* parent;
	/// Node left child
	tRBTreeNode<_Traits>* left;
	/// Node right child
	tRBTreeNode<_Traits>* right;
	/// Node key
	KeyType key;
	/// Node value
	ValueType value; 
};

}

#endif // !KH_STL_TYPE_TREE_NODE_H_
