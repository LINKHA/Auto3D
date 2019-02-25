#ifndef KH_STL_TYPE_RED_BLACK_TREE_ALLOCATOR_H_
#define KH_STL_TYPE_RED_BLACK_TREE_ALLOCATOR_H_

namespace KhSTL {


template<typename _Traits
	, typename _Comp
	, typename _Alloc>
	class tRBTreeAlloc
{
public:
	using KeyType = typename _Traits::KeyType;
	using ValueType = typename _Traits::ValueType;
	using AllocType = _Alloc;
	using NodeType = tRBTreeNode<_Traits>;
public:
	tRBTreeAlloc()
		: _allocator(new AllocType())
	{
	}
	~tRBTreeAlloc()
	{
		delete _allocator;
		_allocator = nullptr;
	}
protected:
	/**
	* @brief : Reserve a node
	*/
	NodeType* getNode()
	{
		auto* newNode = static_cast<NodeType*>(allocation());
		new(newNode) NodeType();
		return newNode;
	}
	/**
	* @brief : Reserve a node with initial value
	*/
	NodeType* getNode(const KeyType& key, const ValueType& value)
	{
		auto* newNode = static_cast<NodeType*>(allocation());
		new(newNode) NodeType(key, value);
		return newNode;
	}

	/**
	* @brief : Free a node
	*/
	void putNode(NodeType* node)
	{
		free(node);
	}
private:
	NodeType* reserve()
	{
		return _allocator->Reserve();
	}
	/**
	* @brief : Reserve and copy-construct an object
	*/
	NodeType* reserve(const NodeType& object)
	{
		return _allocator->Reserve(object);
	}
	/**
	* @brief : Destruct and free an object
	*/
	void free(NodeType* object)
	{
		_allocator->Free(object);
	}

	void* allocation()
	{
		return _allocator->Allocation();
	}

protected:
	AllocType* _allocator;
};


}

#endif // !KH_STL_TYPE_RED_BLACK_TREE_ALLOCATOR_H_
