#ifndef KH_STL_HASH_MAP_H_
#define KH_STL_HASH_MAP_H_

#include "KhMap.h"

namespace KhSTL {
///
///
///
template<typename _Kty, typename _Ty>
using HashMap = KhSTL::tUnorderedMap<_Kty, _Ty>;
///
///
///
template<typename _Kty, typename _Ty> 
using HashMultiMap = KhSTL::tUnorderedMultiMap<_Kty, _Ty>;


}

#endif //!KH_STL_HASH_MAP_H_