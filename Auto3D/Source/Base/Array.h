#pragma once
#include <cassert>
namespace Auto3D
{
template<class _Ty,unsigned _Size> class ArrayIterator
{
public:
	using ValueType = _Ty;
public:
	/// Construct with null pointer
	ArrayIterator()
		: _ptr(nullptr)
		, _index(0)
	{}
	/// Construct with pointer and offset
	explicit ArrayIterator(_Ty* ptr, unsigned offset = 0)
		: _ptr(ptr)
		, _index(offset)
	{}
	/// Return const designated object
	_Ty& operator *()
	{
		return (*operator->());
	}
	/// Return const pointer to class object
	_Ty* operator ->()
	{
		return _ptr + _index;
	}
	/// Rreincrement
	ArrayIterator& operator ++()
	{
		++_index;
		return *this;
	}
	/// Postincrement
	ArrayIterator operator ++(int)
	{
		ArrayIterator temp = *this;
		++*this;
		return temp;
	}
	/**
	/// Predecrement
	*/
	ArrayIterator& operator --()
	{
		--_index;
		return *this;
	}
	/// Postdecrement
	ArrayIterator operator --(int)
	{
		ArrayIterator temp = *this;
		--*this;
		return temp;
	}
	///
	ArrayIterator& operator +=(const unsigned offset)
	{
		_index += offset;
		return *this;
	}
	/// Return this + offset
	ArrayIterator operator +(const unsigned offset) const
	{	// 
		ArrayIterator temp = *this;
		return (temp += offset);
	}
	/// Decrement by integer
	ArrayIterator& operator -=(const unsigned offset)
	{
		return (*this += -offset);
	}
	/// Return this - integer
	ArrayIterator operator -(const unsigned offset) const
	{
		ArrayIterator temp = *this;
		return (temp -= offset);
	}
	/// Return difference of iterators
	unsigned operator -(const ArrayIterator& right) const
	{
		assert(_ptr == right._ptr);
		return (static_cast<unsigned>(_index - right._index));
	}
	/// Subscript
	_Ty& operator [](const unsigned offset) const
	{
		return (*(*this + offset));
	}
	/// Test for iterator equality
	bool operator ==(const ArrayIterator& right) const
	{
		assert(_ptr == right._ptr);
		return (_index == right._index);
	}
	/// Test for iterator inequality
	bool operator !=(const ArrayIterator& right) const
	{
		assert(_ptr == right._ptr);
		return (!(*this == right));
	}
	/// Test if this < right
	bool operator <(const ArrayIterator& right) const
	{
		assert(_ptr == right._ptr);
		return _index < right._index;
	}
	/// Test if this > right
	bool operator >(const ArrayIterator& right) const
	{
		assert(_ptr == right._ptr);
		return right < *this;
	}
	/// Test if this <= right
	bool operator <=(const ArrayIterator& right) const
	{
		assert(_ptr == right._ptr);
		return !(right < *this);
	}
	/// Test if this >= right
	bool operator >=(const ArrayIterator& right) const
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



template<class _Ty,unsigned _Size> class ArrayConstIterator
{
public:
	using ValueType = _Ty;
public:
	/// Construct with null pointer
	ArrayConstIterator()
		: _ptr(nullptr)
		, _index(0)
	{}
	/// Construct with pointer and offset
	explicit ArrayConstIterator(_Ty* ptr, unsigned offset = 0)
		: _ptr(ptr)
		, _index(offset)
	{}
	/// Return const designated object
	const _Ty& operator *()const
	{
		return *operator->();
	}
	/// Return const pointer to class object
	const _Ty* operator ->() const
	{
		return _ptr + _index;
	}
	/// Rreincrement
	ArrayConstIterator& operator ++()
	{
		++_index;
		return *this;
	}
	/// Postincrement
	ArrayConstIterator operator ++(int)
	{
		ArrayConstIterator temp = *this;
		++*this;
		return temp;
	}
	/// Predecrement
	ArrayConstIterator& operator --()
	{
		--_index;
		return *this;
	}
	/// Postdecrement
	ArrayConstIterator operator --(int)
	{
		ArrayConstIterator temp = *this;
		--*this;
		return temp;
	}
	///
	ArrayConstIterator& operator +=(const unsigned offset)
	{
		_index += offset;
		return *this;
	}
	/// Return this + offset
	ArrayConstIterator operator +(const unsigned offset) const
	{	// 
		ArrayConstIterator temp = *this;
		return (temp += offset);
	}
	/// Decrement by integer
	ArrayConstIterator& operator -=(const unsigned offset)
	{
		return (*this += -offset);
	}
	/// Return this - integer
	ArrayConstIterator operator -(const unsigned offset) const
	{
		ArrayConstIterator temp = *this;
		return (temp -= offset);
	}
	/// Return difference of iterators
	unsigned operator -(const ArrayConstIterator& right) const
	{
		assert(_ptr == right._ptr);
		return (static_cast<unsigned>(_index - right._index));
	}
	/// Subscript
	const _Ty& operator [](const unsigned offset) const
	{
		return *(*this + offset);
	}
	/// Test for iterator equality
	bool operator ==(const ArrayConstIterator& right) const
	{
		assert(_ptr == right._ptr);
		return _index == right._index;
	}
	/// Test for iterator inequality
	bool operator !=(const ArrayConstIterator& right) const
	{
		assert(_ptr == right._ptr);
		return !(*this == right);
	}
	/// Test if this < right
	bool operator <(const ArrayConstIterator& right) const
	{
		assert(_ptr == right._ptr);
		return _index < right._index;
	}
	/// Test if this > right
	bool operator >(const ArrayConstIterator& right) const
	{
		assert(_ptr == right._ptr);
		return right < *this;
	}
	/// Test if this <= right
	bool operator <=(const ArrayConstIterator& right) const
	{
		assert(_ptr == right._ptr);
		return !(right < *this);
	}
	/// Test if this >= right
	bool operator >=(const ArrayConstIterator& right) const
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


template <typename _Ty, unsigned _Size> class Array
{
public:
	using Iterator = tArrayIterator<_Ty, _Size>;

	using ConstIterator = tArrayConstIterator<_Ty, _Size>;
public:
	/// Return element at index
	_Ty& operator [](unsigned index)
	{
		assert(index >= 0 && index <= _Size - 1);

		return _elems[index];
	}
	/// Return const element at index
	const _Ty& operator [](unsigned index) const
	{
		assert(index >= 0 && index <= _Size - 1);
		return _elems[index];
	}
	/// Fill by value
	void Fill(const _Ty& value)
	{
		for (unsigned i = 0; i < _Size; i++)
		{
			_elems[i] = value;
		}
	}
	/// Return iterator to the beginning
	Iterator Begin()
	{
		return Iterator(_elems, 0);
	}
	/// Return const iterator to the beginning
	ConstIterator Begin() const
	{
		return ConstIterator(_elems, 0);
	}
	/// Return iterator to the end
	Iterator End()
	{
		return Iterator(_elems, _Size);
	}
	/// Return const iterator to the end
	ConstIterator End() const
	{
		return ConstIterator(_elems, _Size);
	}
	/// Swap with another vector
	void Swap(tArray& rhs)
	{
		KhSTL::Swap(*_elems, *rhs._elems);
	}
	/// Return array max length
	constexpr unsigned Size() const noexcept
	{	// return length of sequence
		return _Size;
	}
	/// Test array is enpty
	constexpr bool Empty() const noexcept
	{
		return _Size == 0;
	}
	/// Return element at index
	_Ty& At(unsigned index)
	{
		assert(index < _Size);
		return _elems[index];
	}
	/// Return const element at index
	const _Ty& At(unsigned index) const
	{
		assert(index < _Size);
		return _elems[index];
	}
	/// Return first element of mutable sequence
	_Ty& Front() noexcept
	{	// 
		return _elems[0];
	}
	/// Return first const element of mutable sequence
	constexpr const _Ty& Front() const noexcept
	{
		return _elems[0];
	}
	/// Return last element of mutable sequence
	_Ty& Back() noexcept
	{
		return _elems[_Size - 1];
	}
	/// Return const last element of mutable sequence
	constexpr const _Ty& Back() const noexcept
	{
		return _elems[_Size - 1];
	}
	/// Return pointer to mutable data array
	_Ty* Data() noexcept
	{
		return _elems;
	}
	/// return const pointer to nonmutable data array
	const _Ty* Data() const noexcept
	{	// 
		return _elems;
	}

private:
	/// Value array
	_Ty _elems[_Size];
};

}