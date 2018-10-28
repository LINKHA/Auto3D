#ifndef KH_STL_TYPE_MAP_H_
#define KH_STL_TYPE_MAP_H_

#include "../../Utility/TypeDef.h"
#include "../../Utility/TypeRBTree.h"
#include "TypeMapTraits.h"

namespace KhSTL {


template<typename _Kty
	, typename _Ty
	, typename _Comp = tLess<_Kty>
	, typename _Alloc = tAllocator<tRBTreeNode<tMapTraits<_Kty, _Ty>>>>
	class tMap : public RBTree<tMapTraits<_Kty, _Ty>, _Comp, _Alloc>
{
public:
	///
	using This = tMap<_Kty, _Ty, _Comp, _Alloc>;
	///
	using Base = RBTree<tMapTraits<_Kty, _Ty>, _Comp, _Alloc>;
	///
	using RBTreeType = RBTree<tMapTraits<_Kty, _Ty>, _Comp, _Alloc>;

	using KeyType = _Kty;

	using ValueType = _Ty;

	using PairType = typename Base::ValueType;

	using KeyCompare = _Comp;

	using Iterator = typename RBTreeType::Iterator;

	using ConstIterator = typename RBTreeType::ConstIterator;
	/// Map reverse iterator
	using ReverseIterator = tReverseIterator<Iterator>;
	/// Map const reverse iterator
	using ConstReverseIterator = tReverseIterator<ConstIterator>;
public:

	tMap() = default;

	tMap(const tMap& map)
	{
		*this = map;
	}
	/**
	* @brief : Move-construct from another map
	*/
	tMap(This && map) noexcept
	{
		Swap(map);
	}

	/**
	* @brief : Aggregate initialization constructor
	*/
	tMap(const std::initializer_list<tPair<_Kty, _Ty>>& list)
		: tMap()
	{
		for (auto it = list.begin(); it != list.end(); it++)
		{
			Insert((*it).first, *it);
		}
	}

	~tMap()
	{
		Clear();
	}
	/**
	* @brief : Assign a map
	*/
	tMap& operator =(const This& rhs)
	{
		// In case of This-assignment do nothing
		if (&rhs != this)
		{
			Clear();
			Insert(rhs);
		}
		return *this;
	}
	/**
	* @brief : Move-assign a map
	*/
	tMap& operator =(This && rhs) noexcept
	{
		assert(&rhs != this);
		Swap(rhs);
		return *this;
	}

	/**
	* @brief : Add-assign a pair
	*/
	tMap& operator +=(const PairType& rhs)
	{
		Insert(rhs);
		return *this;
	}
	/**
	* @brief : Add-assign a map
	*/
	tMap& operator +=(const This& rhs)
	{
		Insert(rhs);
		return *this;
	}
	/**
	* @brief : _Keyest for equality with another map
	*/
	bool operator ==(const This& rhs) const
	{
		if (rhs.Size() != Size())
			return false;

		ConstIterator i = Begin();
		while (i != End())
		{
			ConstIterator j = rhs.Find(i->first);
			if (j == rhs.End() || j->second != i->second)
				return false;
			++i;
		}

		return true;
	}
	/**
	* @brief : _Keyest for inequality with another map
	*/
	bool operator !=(const This& rhs) const
	{
		if (rhs.Size() != Size())
			return true;

		ConstIterator i = Begin();
		while (i != End())
		{
			ConstIterator j = rhs.Find(i->first);
			if (j == rhs.End() || j->second != i->second)
				return true;
			++i;
		}

		return false;
	}
	/**
	* @brief : Index the map. Create a new pair if key not found
	*/
	_Ty& operator [](const _Kty& key)
	{
		Iterator it = Find(key);
		return it != End() ? it->second : Insert(key, _Ty()).second;
	}
	/**
	* @brief : Index the map. Return null if key is not found, does not create a new pair
	*/
	_Ty* operator [](const _Kty& key) const
	{
		Iterator it = Find(key);
		return it != End() ? it.second : 0;
	}

	_Comp KeyComp() const
	{
		return Base::KeyComp();
	}

	bool Empty()
	{
		return Base::Empty();
	}

	unsigned Size()
	{
		return Base::GetSize();
	}

	Iterator Insert(const KeyType& key, const ValueType& value)
	{
		return Base::InsertUnique(key, value);
	}

	Iterator Insert(const PairType& pair)
	{
		return Base::InsertUnique(pair.first, pair);
	}
	/**
	* @brief : Insert a pair. Return iterator and set exists flag according to
	*			whether the key already existed.
	*/
	Iterator Insert(const PairType& pair, bool& exists)
	{
		unsigned oldSize = Size();
		Iterator ret(InsertUnique(pair.first, pair));
		exists = (Size() == oldSize);
		return ret;
	}
	/**
	* @brief : Insert a map
	*/
	void Insert(const This& map)
	{
		ConstIterator it = map.Begin();
		ConstIterator end = map.End();
		while (it != end)
		{
			InsertUnique(it->first, *it);
			++it;
		}
	}
	/**
	* @brief : Insert a pair by iterator. Return iterator to the value
	*/
	Iterator Insert(const ConstIterator& it)
	{
		return InsertUnique(it->first, *it);
	}
	/**
	* @brief : Insert a range by iterators
	*/
	void Insert(const ConstIterator& start, const ConstIterator& end)
	{
		ConstIterator it = start;
		while (it != end)
		{
			InsertUnique(it->first, *it);
			++it;
		}
	}

	Iterator Find(const KeyType& key)
	{
		return Base::Find(key);
	}

	ConstIterator Find(const KeyType& key) const
	{
		return Base::Find(key);
	}

	bool Erase(const _Kty& key)
	{

	}







	void Clear()
	{

		//resetPtrs();

		if (Size())
		{
			for (Iterator i = Begin(); i != End();)
			{
				//FreeNode(static_cast<Node*>(i++.ptr));
				//i.ptr->prev = 0;
				i++;
			}
			//setSize(0);
		}
	}

	inline Iterator Begin() { return Base::Begin(); }

	inline ConstIterator Begin() const { return Base::Begin(); }

	inline Iterator End() { return Base::End(); }

	inline ConstIterator End() const { return Base::End(); }

	inline ReverseIterator RBegin() { return ReverseIterator(End()); }

	inline ConstReverseIterator RBegin() const { return ReverseIterator(End()); }

	inline ReverseIterator REnd() { return ReverseIterator(Begin()); }

	inline ConstReverseIterator REnd() const { return ReverseIterator(Begin()); }

};

}

#endif // KH_STL_TYPE_MAP_H_
