#pragma once
#include <list>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include "ClassID.h"


#define _LIST(t)					std::list<t>
#define _SET(t)						std::set<t,std::less<t>>
#define _VECTOR(t)					std::vector<t>
#define _PAIR(t,x)					std::pair(t,x)
#define M_PAIR(t,x)					std::make_pair(t,x)
#define AUTO_LIST(id,value)			std::list<std::pair<id,value>>
#define AUTO_SET(id,value)			std::set<pair<id,value>,std::less<pair<id,value>>>
#define AUTO_VECTOR(id,value)		std::vector<std::pair<id,value>>
#define AUTO_MAP(key, value)		std::map<key,value,std::less<key>>
#define AUTO_HASH_MAP(key, value)	std::unordered_map<key,value>

