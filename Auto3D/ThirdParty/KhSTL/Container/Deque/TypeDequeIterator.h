#ifndef KH_STL_TYPE_DEQUE_ITERATOR_H_
#define KH_STL_TYPE_DEQUE_ITERATOR_H_

#include "../../Algorithm/TypeAlgorithm.h"

#include "TypeDequeAllocator.h"

//#include <memory>

namespace KhSTL {

template<class _Ty
	, unsigned _Size>
	struct tDequeIterator
{
public:
	using ValueType = _Ty;
	//using DifferenceType = ptrdiff_t;
	using This = tDequeIterator;
	using Iterator = tDequeIterator<_Ty, _Size>;
public:
	

	/**
	* @brief : Transfer buffer position
	*/
	void SetNode(_Ty** newNode)
	{
		node = newNode;
		first = *newNode;
		last = first + BufferSize();
	}

	unsigned BufferSize()
	{
		return _Size != 0 ? _Size : (sizeof(_Ty) < 512 ? unsigned(512 / sizeof(_Size)) : unsigned(1));
	}

	_Ty& operator *() const { return *cur; }
	_Ty* operator ->()const { return &(operator *()); }

	This& operator ++()
	{
		++cur;

		if (cur == last)
		{
			SetNode(node + 1);
			cur = first;
		}
		return *this;
	}

	This& operator ++(int)
	{
		This it = *this;
		++*this;
		return it;
	}

	This& operator --()
	{
		if (cur == first)
		{
			SetNode(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}

	This& operator --(int)
	{
		This it = *this;
		--*this;
		return it;
	}

	This& operator +=(int n)
	{
		int offset = n + (cur - first);

		if (offset >= 0 && offset < BufferSize())
		{
			cur += n;
		}
		else
		{
			int nodeOffset = offset > 0
				? offset / BufferSize()
				: -int((-offset - 1) / BufferSize()) - 1;
			SetNode(node + nodeOffset);
			cur = first + (offset - nodeOffset * BufferSize());
		}
		return *this;
	}

	This operator +(int n) const
	{
		This it = *this;
		return it += n;
	}

	This& operator -=(int n)  
	{ 
		return *this += -n; 
	}

	This operator -(int n) const
	{
		This it = *this;
		return it -= n;
	}

	int operator -(const This& x)
	{
		return int(BufferSize())*(node - x.node - 1) + (cur - first) + (x.last - x.cur);
	}

	_Ty& operator [](int n)const { return *(*this + n); }

	bool operator ==(const This& x) const
	{
		return cur == x.cur;
	}

	bool operator !=(const This& x) const
	{
		return cur != x.cur;
	}

	bool operator <(const This& x) const
	{
		return (node == x.node) ? (cur < x.cur) : (node < x.node);
	}


	_Ty* cur;
	_Ty* first;
	_Ty* last;
	_Ty** node;
};


template<class _Ty
	, unsigned _Size>
	struct tDequeConstIterator
{
	using ValueType = _Ty;
	//using DifferenceType = ptrdiff_t;
	using This = tDequeConstIterator;
	using Iterator = tDequeConstIterator<_Ty, _Size>;

	/**
	* @brief : Transfer buffer position
	*/
	void SetNode(_Ty** newNode)
	{
		node = newNode;
		first = *newNode;
		last = first + DifferenceType(BufferSize());
	}

	unsigned BufferSize()
	{
		return _Size != 0 ? _Size : (sizeof(_Ty) < 512 ? unsigned(512 / sizeof(_Size)) : unsigned(1));
	}

	const _Ty& operator *() const { return *cur; }
	const _Ty* operator ->()const { return &(operator *()); }

	This& operator ++()
	{
		++cur;

		if (cur == last)
		{
			SetNode(node + 1);
			cur = first;
		}
		return *this;
	}

	This& operator ++(int)
	{
		This it = *this;
		++*this;
		return it;
	}

	This& operator --()
	{
		if (cur == first)
		{
			SetNode(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}

	This& operator --(int)
	{
		This it = *this;
		--*this;
		return it;
	}

	This& operator +=(unsigned n)
	{
		unsigned offset = n + (cur - first);

		if (offset >= 0 && offset < unsigned(BufferSize()))
		{
			cur += n;
		}
		else
		{
			unsigned nodeOffset = offset > 0
				? offset / unsigned(BufferSize())
				: -unsigned((-offset - 1) / BufferSize()) - 1;
			SetNode(node + nodeOffset);
			cur = first + (offset - nodeOffset * unsigned(BufferSize()));
		}
		return *this;
	}

	This operator +(unsigned n) const
	{
		This it = *this;
		return it += n;
	}

	This& operator -=(unsigned n)const { return *this += -n; }

	This operator -(unsigned n)const
	{
		This it = *this;
		return it -= n;
	}

	unsigned operator -(const This& x)
	{
		return unsigned(BufferSize())*(node - x.node - 1) + (cur - first) + (x.last - x.cur);
	}

	_Ty& operator [](unsigned n)const { return *(*this + n); }

	bool operator ==(const This& x) const
	{
		return cur == x.cur;
	}

	bool operator !=(const This& x) const
	{
		return cur != x.cur;
	}

	bool operator <(const This& x) const
	{
		return (node == x.node) ? (cur < x.cur) : (node < x.node);
	}


	_Ty* cur;
	_Ty* first;
	_Ty* last;
	_Ty** node;
};

}

#endif // KH_STL_TYPE_DEQUE_ITERATOR_H_