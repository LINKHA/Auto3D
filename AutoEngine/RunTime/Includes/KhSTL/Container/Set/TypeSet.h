#ifndef KH_STL_TYPE_SET_H_
#define KH_STL_TYPE_SET_H_

#include "../../Utility/TypeDef.h"
#include "../../Utility/TypeRBTree.h"
#include "TypeSetTraits.h"

namespace KhSTL {

template<typename _Kty
	, typename _Comp = tLess<_Kty>
	, typename _Alloc = tAllocator<tRBTreeNode<tSetTraits<_Kty>>>>
	class tSet : public RBTree<tSetTraits<_Kty>, _Comp, _Alloc>
{
public:
	using KeyType = _Kty;
	using ValueType = _Kty;
	using KeyCompare = _Comp;
	using Base = RBTree<tSetTraits<_Kty>, _Comp, _Alloc>;
	using RBTreeType = RBTree<tSetTraits<_Kty>, _Comp, _Alloc>;

	using Iterator = typename RBTreeType::Iterator;

	tSet()
	{}

	~tSet()
	{}

	_Comp KeyComp() const { return Base::KeyComp(); }

	Iterator Begin()
	{
		return Base::Begin();
	}
	Iterator End()
	{
		return Base::End();
	}
	bool Empty()
	{
		return Base::Empty();
	}
	unsigned Size()
	{
		return Base::GetSize();
	}
	unsigned max_size()
	{
		return Base::max_size();
	}
	void operator[](const KeyType& k)
	{
		Insert(k);
	}
	Iterator Insert(const KeyType& x)
	{
		return Base::InsertUnique(x,x);
	}
	Iterator Find(const KeyType& x)
	{
		return Base::Find(x);
	}
};


}
#endif // !KH_STL_TYPE_SET_H_