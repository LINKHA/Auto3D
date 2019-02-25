#ifndef KH_STL_SET_H_
#define KH_STL_SET_H_


#ifndef KH_STL_TYPE_SET_H_
#	include "Container/Set/TypeSet.h"
#endif //!KH_STL_TYPE_SET_H_
#ifndef KH_STL_TYPE_MULTI_SET_H_
#	include "Container/Set/TypeMultiSet.h"
#endif //!KH_STL_TYPE_MULTI_SET_H_
#ifndef KH_STL_TYPE_UNORDERED_MULTI_SET_H_
#	include "Container/Set/TypeUnorderedMultiSet.h"
#endif //!KH_STL_TYPE_UNORDERED_MULTI_SET_H_
#ifndef KH_STL_TYPE_UNORDERED_SET_H_
#	include "Container/Set/TypeUnorderedSet.h"
#endif //!KH_STL_TYPE_UNORDERED_SET_H_

namespace KhSTL {
///
///
///
template<typename _Kty
	, typename _Comp = tLess<_Kty>
	, typename _Alloc = tAllocator<tRBTreeNode<tSetTraits<_Kty>>>>
	using Set = KhSTL::tSet<_Kty, _Comp, _Alloc>;
///
///
///
template<typename _Kty
	, typename _Comp = tLess<_Kty>
	, typename _Alloc = tAllocator<tRBTreeNode<tSetTraits<_Kty>>>>
	using MultiSet = KhSTL::tMultiSet<_Kty, _Comp, _Alloc>;
///
///
///
template<typename _Ty> using UnorderedMultiSet = KhSTL::tUnorderedMultiSet<_Ty>;
///
///
///
template<typename _Ty> using UnorderedSet = KhSTL::tUnorderedSet<_Ty>;

}

#endif // !KH_STL_SET_H_
