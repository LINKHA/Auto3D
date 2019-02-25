#ifndef KH_STL_TYPE_ARRAY_H_
#define KH_STL_TYPE_ARRAY_H_
#include "TypeArrayIterator.h"
#include "../../Utility/TypeReverseIterator.h"
#include <initializer_list>

namespace KhSTL {


template <typename _Ty
	,unsigned _Size> 
	class tArray 
{
public:
	using ValueType = _Ty;

	using Iterator = tArrayIterator<_Ty, _Size>;

	using ConstIterator = tArrayConstIterator<_Ty, _Size>;

	using ReverseIterator = tReverseIterator<Iterator>;

	using ConstReverseIterator = tReverseIterator<ConstIterator>;
public:
	/**
	* @brief : Return element at index
	*/
	_Ty& operator [](unsigned index)
	{
		assert(index >= 0 && index <= _Size - 1);

		return _elems[index];
	}
	/**
	* @brief : Return const element at index
	*/
	const _Ty& operator [](unsigned index) const
	{
		assert(index >= 0 && index <= _Size - 1);
		return _elems[index];
	}
	/**
	* @brief : Fill by value
	*/
	void Fill(const _Ty& value)
	{	
		for (unsigned i = 0; i < _Size; i++)
		{
			_elems[i] = value;
		}
	}
	/**
	* @brief : Return iterator to the beginning
	*/
	Iterator Begin()
	{
		return Iterator(_elems, 0);
	}
	/**
	* @brief : Return const iterator to the beginning
	*/
	ConstIterator Begin() const
	{
		return ConstIterator(_elems, 0);
	}
	/**
	* @brief : Return iterator to the end
	*/
	Iterator End()
	{
		return Iterator(_elems, _Size);
	}
	/**
	* @brief : Return const iterator to the end
	*/
	ConstIterator End() const
	{
		return ConstIterator(_elems, _Size);
	}
	/**
	* @brief : Swap with another vector
	*/
	void Swap(tArray& rhs)
	{
		KhSTL::Swap(*_elems, *rhs._elems);
	}
	/**
	* @brief : Return array max length
	*/
	constexpr unsigned Size() const noexcept
	{	// return length of sequence
		return _Size;
	}
	/**
	* @brief : Test array is enpty
	*/
	constexpr bool Empty() const noexcept
	{	
		return _Size == 0;
	}
	/**
	* @brief : Return element at index
	*/
	_Ty& At(unsigned index)
	{
		assert(index < _Size);
		return _elems[index];
	}
	/**
	* @brief : Return const element at index
	*/
	const _Ty& At(unsigned index) const
	{
		assert(index < _Size);
		return _elems[index];
	}
	/**
	* @brief : Return first element of mutable sequence
	*/
	_Ty& Front() noexcept 
	{	// 
		return _elems[0];
	}
	/**
	* @brief : Return first const element of mutable sequence
	*/
	constexpr const _Ty& Front() const noexcept 
	{
		return _elems[0];
	}
	/**
	* @brief : Return last element of mutable sequence
	*/
	_Ty& Back() noexcept
	{
		return _elems[_Size - 1];
	}
	/**
	* @brief : Return const last element of mutable sequence
	*/
	constexpr const _Ty& Back() const noexcept 
	{
		return _elems[_Size - 1];
	}
	/**
	* @brief : Return pointer to mutable data array
	*/
	_Ty* Data() noexcept
	{	
		return _elems;
	}
	/**
	* @brief : return const pointer to nonmutable data array
	*/
	const _Ty* Data() const noexcept
	{	// 
		return _elems;
	}
	
private:
	/// Value array
	_Ty _elems[_Size];
};



}
#endif //!KH_STL_TYPE_ARRAY_H_