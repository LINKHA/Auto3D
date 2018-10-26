#pragma once

#include <KhSTL/KhSTL>

//#include <list>
//#include <vector>
//#include <set>
//#include <map>
//#include <unordered_map>

#define _LIST(t)					KhSTL::List<t>
#define _SET(t)						KhSTL::Set<t,KhSTL::Less<t>>
#define _VECTOR(t)					KhSTL::Vector<t>
#define _PAIR(t,x)					KhSTL::Pair(t,x)
#define M_PAIR(t,x)					KhSTL::MakePair(t,x)
#define AUTO_LIST(id,value)			KhSTL::List<KhSTL::Pair<id,value>>
#define AUTO_SET(id,value)			KhSTL::Set<KhSTL::Pair<id,value>,KhSTL::Less<KhSTL::Pair<id,value>>>
#define AUTO_VECTOR(id,value)		KhSTL::Vector<KhSTL::Pair<id,value>>
#define AUTO_MAP(key, value)		KhSTL::Map<key,value,KhSTL::Less<KhSTL::tPair<key,value>>>
#define AUTO_HASH_MAP(key, value)	KhSTL::UnorderedMap<key,value>
#define AUTO_HASH_SET(key)			KhSTL::UnorderedSet<key>
#define __String					std::string
//#define _LIST(t)					std::list<t>
//#define _SET(t)						std::set<t,std::less<t>>
//#define _VECTOR(t)					std::vector<t>
//#define _PAIR(t,x)					std::pair(t,x)
//#define M_PAIR(t,x)					std::make_pair(t,x)
//#define AUTO_LIST(id,value)			std::list<std::pair<id,value>>
//#define AUTO_SET(id,value)			std::set<pair<id,value>,std::less<pair<id,value>>>
//#define AUTO_VECTOR(id,value)		std::vector<std::pair<id,value>>
//#define AUTO_MAP(key, value)		std::map<key,value,std::less<key>>
//#define AUTO_HASH_MAP(key, value)	std::unordered_map<key,value>