#ifndef KH_STL_LIST_ALLOCTOR_H_
#define KH_STL_LIST_ALLOCTOR_H_

#include "TypeListValue.h"

namespace KhSTL {

template <typename _Ty
	, typename _Alloc>
class tListAlloc : public tListValue<_Ty>
{
public:
	using Base = tListValue<_Ty>;
	using AllocType = typename _Alloc::ValueType;
	using Value = tListValue<_Ty>;
public:
	tListAlloc()
		: _allocator(new _Alloc())
	{
		Value::_head = Value::_tail = reserveNode();
	}
	
	explicit tListAlloc(unsigned size)
		: _allocator(new _Alloc(size + 1))
	{
		Value::_head = Value::_tail = reserveNode();
	}
	virtual ~tListAlloc()
	{
		freeNode(Value::tail());
		delete _allocator;
		_allocator = nullptr;
	}
protected:
	/**
	* @brief : Allocate and insert a node into the list
	*/
	void insertNode(tListNode<_Ty>* dest, const _Ty& value)
	{
		if (!dest)
			return;

		tListNode<_Ty>* newNode = reserveNode(value);
		tListNode<_Ty>* prev = dest->Prev();
		newNode->next = dest;
		newNode->prev = prev;
		if (prev)
			prev->next = newNode;
		dest->prev = newNode;

		// Reassign the head node if necessary
		if (dest == Value::head())
			Value::_head = newNode;

		++Value::_size;
	}
	/**
	* @brief : Erase and free a node. Return pointer to the next node,
	or to the end if could not erase
	*/
	tListNode<_Ty>* eraseNode(tListNode<_Ty>* node)
	{
		// The tail node can not be removed
		if (!node || node == Value::_tail)
			return Value::tail();

		tListNode<_Ty>* prev = node->Prev();
		tListNode<_Ty>* next = node->Next();
		if (prev)
			prev->next = next;
		next->prev = prev;

		// Reassign the head node if necessary
		if (node == Value::head())
			Value::_head = next;

		freeNode(node);
		--Value::_size;

		return next;
	}
	/**
	* @brief : Reserve a node
	*/
	tListNode<_Ty>* reserveNode()
	{
		auto* newNode = static_cast<tListNode<_Ty>*>(allocation());
		new(newNode) tListNode<_Ty>();
		return newNode;
	}
	/**
	* @brief : Reserve a node with initial value
	*/
	tListNode<_Ty>* reserveNode(const _Ty& value)
	{
		auto* newNode = static_cast<tListNode<_Ty>*>(allocation());
		new(newNode) tListNode<_Ty>(value);
		return newNode;
	}
	/**
	* @brief : Free a node
	*/
	void freeNode(tListNode<_Ty>* node)
	{
		free(node);
	}
private:
	AllocType* reserve()
	{
		return _allocator->Reserve();
	}
	/**
	* @brief : Reserve and copy-construct an object
	*/
	AllocType* reserve(const AllocType& object)
	{
		return _allocator->Reserve(object);
	}
	/**
	* @brief : Destruct and free an object
	*/
	void free(AllocType* object)
	{
		_allocator->Free(object);
	}

	void* allocation()
	{
		return _allocator->Allocation();
	}

protected:
	_Alloc* _allocator;
};

}
#endif //!KH_STL_LIST_ALLOCTOR_H_s




