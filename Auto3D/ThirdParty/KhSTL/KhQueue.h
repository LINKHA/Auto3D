#ifndef KH_STL_QUEUE_H_
#define KH_STL_QUEUE_H_

#ifndef KH_STL_TYPE_QUEUE_H_
#	include "Container/Queue/TypeQueue.h"
#endif //!KH_STL_TYPE_QUEUE_H_

namespace KhSTL {
///
///
///
template<typename _Ty> using Queue = KhSTL::tQueue<_Ty>;

}


#endif //!KH_STL_QUEUE_H_