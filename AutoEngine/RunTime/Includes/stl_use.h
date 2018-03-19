#ifndef STL_USE_H_
#define STL_USE_H_
#include <list>
#include <vector>
#include <set>
#include <map>




#define AUTO_LIST(t) std::list<t>
#define AUTO_SET(t)	std::set<t,std::less<t>>
#define AUTO_MAP(key, value)	std::map<key,value,std::less<key>>
#define AUTO_VECTOR(t) std::vector<t>






#endif //! STL_USE_H_
