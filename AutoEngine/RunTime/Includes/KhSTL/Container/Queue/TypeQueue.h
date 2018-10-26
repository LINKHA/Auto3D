#ifndef KH_STL_TYPE_QUEUE_H_
#define KH_STL_TYPE_QUEUE_H_

#include "../Deque/TypeDeque.h"

namespace KhSTL {


template<typename _Ty
	,typename _Container = tDeque<_Ty>>
class tQueue
{
public:
	/**
	* @brief :  Construct Empty
	*/
	tQueue()
		:_container()
	{}
	/**
	* @brief : Insert element at beginning
	*/
	void Push(_Ty && _Val)
	{	// 
		_container.PushBack(_Val);
	}
	/**
	* @brief : Insert element at beginning
	*/
	void Push(const _Ty& value)
	{	//
		_container.PushBack(value);
	}
	/**
	* @brief :
	*/
	void Pop()
	{	// erase element at end
		_container.PopFront();
	}
	/**
	* @brief :
	*/
	const _Ty& GetContainer() const
	{	// get reference to container
		return _container;
	}

	/**
	* @brief :
	*/
	template<typename... _TyArg>
	_Ty Emplace(_TyArg&&... value)
	{	// insert element at beginning
		_container.EmplaceBack(std::forward<_TyArg>(value)...);
	}
	/**
	* @brief :
	*/
	bool Empty() const
	{	// test if queue is Empty
		return _container.Empty();
	}
	/**
	* @brief :
	*/
	unsigned Size() const
	{	// return length of queue
		return _container.Size();
	}
	/**
	* @brief :
	*/
	_Ty& Front()
	{	// return first element of mutable queue
		return _container.Front();
	}
	/**
	* @brief :
	*/
	const _Ty& Front() const
	{	// return first element of nonmutable queue
		return _container.Front();
	}
	/**
	* @brief :
	*/
	_Ty& Back()
	{	// return last element of mutable queue
		return _container.Back();
	}
	/**
	* @brief :
	*/
	const _Ty& Back() const
	{	// return last element of nonmutable queue
		return _container.Back();
	}
	/**
	* @brief :
	*/
	
	void swap(tQueue& rhs)
	{	// exchange contents with _Right
		//_Swap_adl(_container, _Right.c);
	}

protected:
	_Container _container;	// the underlying container
};


}

#endif // !KH_STL_TYPE_QUEUE_H_