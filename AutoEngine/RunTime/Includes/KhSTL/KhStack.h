#ifndef KH_STL_STACK_H_
#define KH_STL_STACK_H_

#ifndef KH_STL_TYPE_STACK_H_
#	include "Container/Stack/TypeStack.h"
#endif //!KH_STL_TYPE_STACK_H_

namespace KhSTL {
///
///
///
template<typename _Ty> using Stack = KhSTL::tStack<_Ty>;

}

#endif //!KH_STL_STACK_H_