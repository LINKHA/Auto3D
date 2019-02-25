#ifndef KH_STL_TYPE_ITERATOR_H_
#define KH_STL_TYPE_ITERATOR_H_

namespace KhSTL {
template <typename _Ty>
	class tIterator
{
	using This = tIterator<_Ty>;
	using ValueType = _Ty;
public:
	/**
	* @brief : Construct
	*/
	tIterator()
		: ptr(nullptr)
	{}
	/**
	* @brief : Construct with an object pointer
	*/
	explicit tIterator(_Ty* sptr)
		: ptr(sptr)
	{}
	/**
	* @brief : Point to the object
	*/
	_Ty* operator ->() const { return ptr; }
	/**
	* @brief : Dereference the object
	*/
	_Ty& operator *() const { return *ptr; }
	/**
	* @brief : Preincrement the pointer
	*/
	tIterator<_Ty>& operator ++()
	{
		++ptr;
		return *this;
	}
	/**
	* @brief : Postincrement the pointer
	*/
	tIterator<_Ty> operator ++(int)
	{
		tIterator<_Ty> it = *this;
		++ptr;
		return it;
	}
	/**
	* @brief : Predecrement the pointer
	*/
	tIterator<_Ty>& operator --()
	{
		--ptr;
		return *this;
	}
	/**
	* @brief : Postdecrement the pointer
	*/
	tIterator<_Ty> operator --(int)
	{
		tIterator<_Ty> it = *this;
		--ptr;
		return it;
	}
	/**
	* @brief : Add an offset to the pointer
	*/
	tIterator<_Ty>& operator +=(int value){ ptr += value;return *this;}
	/**
	* @brief : Subtract an offset from the pointer
	*/
	tIterator<_Ty>& operator -=(int value){ ptr -= value; return *this;}
	/**
	* @brief : Add an offset to the pointer
	*/
	tIterator<_Ty> operator +(int value) const { return tIterator<_Ty>(ptr + value); }
	/**
	* @brief : Subtract an offset from the pointer
	*/
	tIterator<_Ty> operator -(int value) const { return tIterator<_Ty>(ptr - value); }
	/**
	* @brief : Calculate offset to another iterator
	*/
	int operator -(const tIterator& rhs) const { return (int)(ptr - rhs.ptr); }
	/**
	* @brief : Test for equality with another iterator
	*/
	bool operator ==(const tIterator& rhs) const { return ptr == rhs.ptr; }
	/**
	* @brief : Test for inequality with another iterator
	*/
	bool operator !=(const tIterator& rhs) const { return ptr != rhs.ptr; }
	/**
	* @brief : Test for less than with another iterator
	*/
	bool operator <(const tIterator& rhs) const { return ptr < rhs.ptr; }
	/**
	* @brief : Test for greater than with another iterator
	*/
	bool operator >(const tIterator& rhs) const { return ptr > rhs.ptr; }
	/**
	* @brief : Test for less than or equal with another iterator
	*/
	bool operator <=(const tIterator& rhs) const { return ptr <= rhs.ptr; }
	/**
	* @brief : Test for greater than or equal with another iterator
	*/
	bool operator >=(const tIterator& rhs) const { return ptr >= rhs.ptr; }

	/// Pointer.
	_Ty* ptr;
};

template <typename _Ty> class tConstIterator
{
	using This = tConstIterator<_Ty>;
public:
	/**
	* @brief : Construct
	*/
	tConstIterator()
		: ptr(0)
	{}
	/**
	* @brief : Construct with an object pointer
	*/
	explicit tConstIterator(const _Ty* sptr)
		: ptr(sptr)
	{}
	/**
	* @brief : Construct from a non-const iterator
	*/
	tConstIterator(const tConstIterator<_Ty>& rhs) 
		: ptr(rhs.ptr)
	{}
	/**
	* @brief : Assign from a non-const iterator
	*/
	tConstIterator<_Ty>& operator =(const tConstIterator<_Ty>& rhs)
	{
		ptr = rhs.ptr;
		return *this;
	}
	/**
	* @brief : Point to the object
	*/
	const _Ty* operator ->() const { return ptr; }
	/**
	* @brief : Dereference the object
	*/
	const _Ty& operator *() const { return *ptr; }
	/**
	* @brief : Preincrement the pointer
	*/
	tConstIterator<_Ty>& operator ++()
	{
		++ptr;
		return *this;
	}
	/**
	* @brief : Postincrement the pointer
	*/
	tConstIterator<_Ty> operator ++(int)
	{
		tConstIterator<_Ty> it = *this;
		++ptr;
		return it;
	}
	/**
	* @brief : Predecrement the pointer
	*/
	tConstIterator<_Ty>& operator --()
	{
		--ptr;
		return *this;
	}
	/**
	* @brief : Postdecrement the pointer
	*/
	tConstIterator<_Ty> operator --(int)
	{
		tConstIterator<_Ty> it = *this;
		--ptr;
		return it;
	}
	/**
	* @brief : Add an offset to the pointer
	*/
	tConstIterator<_Ty>& operator +=(int value)
	{
		ptr += value;
		return *this;
	}
	/**
	* @brief : Subtract an offset from the pointer
	*/
	tConstIterator<_Ty>& operator -=(int value)
	{
		ptr -= value;
		return *this;
	}
	/**
	* @brief : Add an offset to the pointer
	*/
	tConstIterator<_Ty> operator +(int value) const { return tConstIterator<_Ty>(ptr + value); }
	/**
	* @brief : Subtract an offset from the pointer
	*/
	tConstIterator<_Ty> operator -(int value) const { return tConstIterator<_Ty>(ptr - value); }
	/**
	* @brief : Calculate offset to another iterator
	*/
	int operator -(const tConstIterator& rhs) const { return (int)(ptr - rhs.ptr); }
	/**
	* @brief : Test for equality with another iterator
	*/
	bool operator ==(const tConstIterator& rhs) const { return ptr == rhs.ptr; }
	/**
	* @brief : Test for inequality with another iterator
	*/
	bool operator !=(const tConstIterator& rhs) const { return ptr != rhs.ptr; }
	/**
	* @brief : Test for less than with another iterator
	*/
	bool operator <(const tConstIterator& rhs) const { return ptr < rhs.ptr; }
	/**
	* @brief : Test for greater than with another iterator
	*/
	bool operator >(const tConstIterator& rhs) const { return ptr > rhs.ptr; }
	/**
	* @brief : Test for less than or equal with another iterator
	*/
	bool operator <=(const tConstIterator& rhs) const { return ptr <= rhs.ptr; }
	/**
	* @brief : Test for greater than or equal with another iterator
	*/
	bool operator >=(const tConstIterator& rhs) const { return ptr >= rhs.ptr; }

	/// Pointer.
	const _Ty* ptr;
};


}
#endif //!KH_STL_TYPE_ITERATOR_H_
