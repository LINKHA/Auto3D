#ifndef KH_STL_HASH_SET_H_
#define KH_STL_HASH_SET_H_

#include "KhSet.h"

namespace KhSTL {
///
///
///
template<typename _Kty>
using HashSet = KhSTL::tUnorderedSet<_Kty>;
///
///
///
template<typename _Kty>
using UnorderedSet = KhSTL::tUnorderedSet<_Kty>;
///
///
///
template<typename _Kty> 
using HashMultiSet = KhSTL::tUnorderedMultiSet<_Kty>;
///
///
///
template<typename _Kty> 
using UnorderedMultiSet = KhSTL::tUnorderedMultiSet<_Kty>;

}

#endif //!KH_STL_HASH_SET_H_