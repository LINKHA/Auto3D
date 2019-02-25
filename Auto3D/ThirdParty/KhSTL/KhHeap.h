#ifndef KH_STL_HEAP_H_
#define KH_STL_HEAP_H_

#ifndef KH_STL_TYPE_HEAP_H_
#	include "Container/Heap/TypeHeap.h"
#endif //!KH_STL_TYPE_HEAP_H_

namespace KhSTL {

template<typename _Ty
	, typename _Alloc = tAllocator<_Ty>>
	using Heap = KhSTL::tHeap<_Ty,_Alloc>;




}

#endif //!KH_STL_HEAP_H_
