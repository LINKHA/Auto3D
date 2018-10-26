#ifndef KH_STL_VECTOR_H_
#define KH_STL_VECTOR_H_

#ifndef KH_STL_TYPE_VECTOR_H_
#	include "Container/TypeVector.h"
#endif


namespace KhSTL {
///
///
///
template<typename _Ty> using Vector = KhSTL::tVector<_Ty>;

}


#endif //!KH_STL_VECTOR_H_
