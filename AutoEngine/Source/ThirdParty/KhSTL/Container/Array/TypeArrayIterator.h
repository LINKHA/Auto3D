#ifndef KH_STL_ARRAY_ITERATOR_H_
#define KH_STL_ARRAY_ITERATOR_H_

#include <cassert>

namespace KhSTL {


template<class _Ty,
	unsigned _Size>
	class tArrayIterator
{
public:
	using ValueType = _Ty;
public:
	/**
	* @brief : Construct with null pointer
	*/
	tArrayIterator()
		: _ptr(nullptr)
		, _index(0)
	{}
	/**
	* @brief : Construct with pointer and offset
	*/
	explicit tArrayIterator(_Ty* ptr, unsigned offset = 0)
		: _ptr(ptr)
		, _index(offset)
	{}
	/**
	* @brief : Return const designated object
	*/
	_Ty& operator *()
	{
		return (*operator->());
	}
	/**
	* @brief : Return const pointer to class object
	*/
	_Ty* operator ->()
	{
		return _ptr + _index;
	}
	/**
	* @brief : Rreincrement
	*/
	tArrayIterator& operator ++()
	{
		++_index;
		return *this;
	}
	/**
	* @brief : Postincrement
	*/
	tArrayIterator operator ++(int)
	{
		tArrayIterator temp = *this;
		++*this;
		return temp;
	}
	/**
	* @brief : Predecrement
	*/
	tArrayIterator& operator --()
	{
		--_index;
		return *this;
	}
	/**
	* @brief : Postdecrement
	*/
	tArrayIterator operator --(int)
	{
		tArrayIterator temp = *this;
		--*this;
		return temp;
	}
	/**
	* @brief :
	*/
	tArrayIterator& operator +=(const unsigned offset)
	{
		_index += offset;
		return *this;
	}
	/**
	* @brief : Return this + offset
	*/
	tArrayIterator operator +(const unsigned offset) const
	{	// 
		tArrayIterator temp = *this;
		return (temp += offset);
	}
	/**
	* @brief : Decrement by integer
	*/
	tArrayIterator& operator -=(const unsigned offset)
	{
		return (*this += -offset);
	}
	/**
	* @brief : Return this - integer
	*/
	tArrayIterator operator -(const unsigned offset) const
	{
		tArrayIterator temp = *this;
		return (temp -= offset);
	}
	/**
	* @brief : Return difference of iterators
	*/
	unsigned operator -(const tArrayIterator& right) const
	{
		assert(_ptr == right._ptr);
		return (static_cast<unsigned>(_index - right._index));
	}
	/**
	* @brief : Subscript
	*/
	_Ty& operator [](const unsigned offset) const
	{
		return (*(*this + offset));
	}
	/**
	* @brief : Test for iterator equality
	*/
	bool operator ==(const tArrayIterator& right) const
	{
		assert(_ptr == right._ptr);
		return (_index == right._index);
	}
	/**
	* @brief : Test for iterator inequality
	*/
	bool operator !=(const tArrayIterator& right) const
	{
		assert(_ptr == right._ptr);
		return (!(*this == right));
	}
	/**
	* @brief : Test if this < right
	*/
	bool operator <(const tArrayIterator& right) const
	{
		assert(_ptr == right._ptr);
		return _index < right._index;
	}
	/**
	* @brief : Test if this > right
	*/
	bool operator >(const tArrayIterator& right) const
	{
		assert(_ptr == right._ptr);
		return right < *this;
	}
	/**
	* @brief : Test if this <= right
	*/
	bool operator <=(const tArrayIterator& right) const
	{
		assert(_ptr == right._ptr);
		return !(right < *this);
	}
	/**
	* @brief : Test if this >= right
	*/
	bool operator >=(const tArrayIterator& right) const
	{
		assert(_ptr == right._ptr);
		return !(*this < right);
	}

	constexpr const _Ty* UnWrapped() const
	{
		return _ptr + _index;
	}

	constexpr void SeekTo(const _Ty it)
	{
		_index = static_cast<unsigned>(it - _ptr);
	}
private:
	_Ty* _ptr;
	unsigned _index;
};



template<class _Ty,
	unsigned _Size>
	class tArrayConstIterator
{
public:
	using ValueType = _Ty;
public:
	/**
	* @brief : Construct with null pointer
	*/
	tArrayConstIterator()
		: _ptr(nullptr)
		, _index(0)
	{}
	/**
	* @brief : Construct with pointer and offset
	*/
	explicit tArrayConstIterator(_Ty* ptr, unsigned offset = 0)
		: _ptr(ptr)
		, _index(offset)
	{}
	/**
	* @brief : Return const designated object
	*/
	const _Ty& operator *()const
	{
		return *operator->();
	}
	/**
	* @brief : Return const pointer to class object
	*/
	const _Ty* operator ->() const
	{
		return _ptr + _index;
	}
	/**
	* @brief : Rreincrement
	*/
	tArrayConstIterator& operator ++()
	{
		++_index;
		return *this;
	}
	/**
	* @brief : Postincrement
	*/
	tArrayConstIterator operator ++(int)
	{
		tArrayConstIterator temp = *this;
		++*this;
		return temp;
	}
	/**
	* @brief : Predecrement
	*/
	tArrayConstIterator& operator --()
	{
		--_index;
		return *this;
	}
	/**
	* @brief : Postdecrement
	*/
	tArrayConstIterator operator --(int)
	{
		tArrayConstIterator temp = *this;
		--*this;
		return temp;
	}
	/**
	* @brief :
	*/
	tArrayConstIterator& operator +=(const unsigned offset)
	{
		_index += offset;
		return *this;
	}
	/**
	* @brief : Return this + offset
	*/
	tArrayConstIterator operator +(const unsigned offset) const
	{	// 
		tArrayConstIterator temp = *this;
		return (temp += offset);
	}
	/**
	* @brief : Decrement by integer
	*/
	tArrayConstIterator& operator -=(const unsigned offset)
	{
		return (*this += -offset);
	}
	/**
	* @brief : Return this - integer
	*/
	tArrayConstIterator operator -(const unsigned offset) const
	{
		tArrayConstIterator temp = *this;
		return (temp -= offset);
	}
	/**
	* @brief : Return difference of iterators
	*/
	unsigned operator -(const tArrayConstIterator& right) const
	{
		assert(_ptr == right._ptr);
		return (static_cast<unsigned>(_index - right._index));
	}
	/**
	* @brief : Subscript
	*/
	const _Ty& operator [](const unsigned offset) const
	{
		return *(*this + offset);
	}
	/**
	* @brief : Test for iterator equality
	*/
	bool operator ==(const tArrayConstIterator& right) const
	{
		assert(_ptr == right._ptr);
		return _index == right._index;
	}
	/**
	* @brief : Test for iterator inequality
	*/
	bool operator !=(const tArrayConstIterator& right) const
	{
		assert(_ptr == right._ptr);
		return !(*this == right);
	}
	/**
	* @brief : Test if this < right
	*/
	bool operator <(const tArrayConstIterator& right) const
	{
		assert(_ptr == right._ptr);
		return _index < right._index;
	}
	/**
	* @brief : Test if this > right
	*/
	bool operator >(const tArrayConstIterator& right) const
	{
		assert(_ptr == right._ptr);
		return right < *this;
	}
	/**
	* @brief : Test if this <= right
	*/
	bool operator <=(const tArrayConstIterator& right) const
	{
		assert(_ptr == right._ptr);
		return !(right < *this);
	}
	/**
	* @brief : Test if this >= right
	*/
	bool operator >=(const tArrayConstIterator& right) const
	{
		assert(_ptr == right._ptr);
		return !(*this < right);
	}

	constexpr const _Ty* UnWrapped() const
	{
		return _ptr + _index;
	}

	constexpr void SeekTo(const _Ty it)
	{
		_index = static_cast<unsigned>(it - _ptr);
	}
private:
	const _Ty* _ptr;
	unsigned _index;
};



}
#endif //!KH_STL_ARRAY_ITERATOR_H_