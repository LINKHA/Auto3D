#ifndef KH_STL_TYPE_REVERSE_ITERATOR_H_
#define KH_STL_TYPE_REVERSE_ITERATOR_H_

namespace KhSTL {

template <typename _Iterator>
class tReverseIterator
{	// wrap iterator to run it backwards
public:
	using ValueType = typename _Iterator::ValueType;
public:
	tReverseIterator()
		: _current()
	{	// construct with value-initialized wrapped iterator
	}

	explicit tReverseIterator(_Iterator rhs)
		: _current(rhs)
	{	// construct wrapped iterator from _Right
	}

	template<class _Other>
	tReverseIterator(const tReverseIterator<_Other>& rhs)
		: _current(rhs.Base())
	{	// initialize with compatible Base
	}

	template<class _Other>
	tReverseIterator& operator =(const tReverseIterator<_Other>& rhs)
	{	// assign from compatible Base
		_current = rhs.Base();
		return *this;
	}

	_Iterator Base() const
	{	// return wrapped iterator
		return _current;
	}

	ValueType& operator *() const
	{	// return designated value
		_Iterator temp = _current;
		temp--;
		return *temp;
	}

	ValueType* operator ->() const
	{	// return pointer to class object
		_Iterator tmp = _current;
		tmp--;
		ValueType valueTmp = *tmp;
		return &valueTmp;
	}

	tReverseIterator& operator ++()
	{	// preincrement
		--_current;
		return *this;
	}

	tReverseIterator operator ++(int)
	{	// postincrement
		tReverseIterator _Tmp = *this;
		--_current;
		return (_Tmp);
	}

	tReverseIterator& operator --()
	{	// predecrement
		++_current;
		return (*this);
	}

	tReverseIterator operator --(int)
	{	// postdecrement
		tReverseIterator _Tmp = *this;
		++_current;
		return (_Tmp);
	}

	// N.B. functions valid for random-access iterators only beyond this point

	tReverseIterator& operator +=(int offset)
	{	// Increment by integer
		_current -= offset;
		return (*this);
	}

	tReverseIterator operator +(int offset) const
	{	// return this + integer
		return (tReverseIterator(_current - offset));
	}

	tReverseIterator& operator -=(int offset)
	{	// Decrement by integer
		_current += offset;
		return (*this);
	}

	tReverseIterator operator -(int offset) const
	{	// return this - integer
		return (tReverseIterator(_current + offset));
	}

	ValueType& operator [](int offset) const
	{	// subscript
		return (*(*this + offset));
	}

	bool operator ==(const tReverseIterator& right) const
	{
		return _current == right._current;
	}
	/**
	* @brief : Test for iterator inequality
	*/
	bool operator !=(const tReverseIterator& right) const
	{
		return _current != right._current;
	}


protected:
	_Iterator _current;	// the wrapped iterator
};

}


#endif // !KH_STL_TYPE_REVERSE_ITERATOR_H_
