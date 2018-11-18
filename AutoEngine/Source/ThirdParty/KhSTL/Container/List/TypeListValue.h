#ifndef KH_STL_TYPE_LIST_VALUE_H_
#define KH_STL_TYPE_LIST_VALUE_H_

namespace KhSTL {

/**
* Doubly-linked list node Base class
*/
struct tListNodeBase
{
	/**
	* @brief : Construct
	*/
	tListNodeBase()
		: prev(nullptr)
		, next(nullptr)
	{}

	/// Previous node.
	tListNodeBase* prev;
	/// Next node.
	tListNodeBase* next;
};

/**
* List node
*/
template<typename _Ty>
struct tListNode : public tListNodeBase
{
	/**
	* @brief : Construct undefined
	*/
	tListNode() = default;
	/**
	* @brief : Construct with value
	*/
	explicit tListNode(const _Ty& svalue)
		: value(svalue)
	{}
	/**
	* @brief : Return next node
	*/
	tListNode* Next() const { return static_cast<tListNode*>(next); }
	/**
	* @brief : Return previous node
	*/
	tListNode* Prev() { return static_cast<tListNode*>(prev); }

	/// Node value
	_Ty value;
};


template <typename _Ty> class tListValue
{
public:
	tListValue()
		: _head(nullptr)
		, _tail(nullptr)
		, _size(0)
	{
	}
	tListValue(unsigned size) // Reserve the tail node + initial capacity according to the list's size
		: _head(nullptr)
		, _tail(nullptr)
		, _size(0)
	{
	}
	virtual ~tListValue()
	{
		
	}

	/**
	* @brief : Swap with another list
	*/
	void Swap(tListValue& rhs)
	{
		KhSTL::Swap(_head, rhs._head);
		KhSTL::Swap(_tail, rhs._tail);
		KhSTL::Swap(_size, rhs._size);
		KhSTL::Swap(Base::_allocator, rhs.Base::_allocator);
	}
protected:
	/**
	* @brief : Return the head node
	*/
	tListNode<_Ty>* head() const { return static_cast<tListNode<_Ty>*>(_head); }
	/**
	* @brief : Return the tail node
	*/
	tListNode<_Ty>* tail() const { return static_cast<tListNode<_Ty>*>(_tail); }
	
protected:
	/// Head node pointer.
	tListNode<_Ty>* _head;
	/// Tail node pointer.
	tListNode<_Ty>* _tail;
	/// Number of nodes.
	unsigned _size;

};

template <typename _Ty> inline
	void Swap(tListValue<_Ty>& rhs, tListValue<_Ty>& lfs)
{
	rhs.Swap(lfs);
}

}
#endif //!KH_STL_TYPE_LIST_VALUE_H_
