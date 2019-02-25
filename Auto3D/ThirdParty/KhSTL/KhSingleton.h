#ifndef KH_STL_SINGLETON_H_
#define KH_STL_SINGLETON_H_

#ifndef KH_STL_TYPE_SINGLETON_H_
#	include "Utility/TypeSingleton.h"
#endif //!KH_STL_TYPE_SINGLETON_H_

namespace KhSTL {
///
///
///
template<typename _Ty> using Singleton = KhSTL::tSingleton<_Ty>;

}


#endif // KH_STL_SINGLETON_H_
