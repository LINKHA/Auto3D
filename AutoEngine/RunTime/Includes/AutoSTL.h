#pragma once
#include <list>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>


#define LIST(t)					std::list<t>
#define SET(t)						std::set<t,std::less<t>>
#define VECTOR(t)					std::vector<t>
#define PAIR(t,x)					std::pair(t,x)
#define MAKE_PAIR(t,x)					std::make_pair(t,x)
#define PAIR_LIST(id,value)			std::list<std::pair<id,value>>
#define PAIR_SET(id,value)			std::set<pair<id,value>,std::less<pair<id,value>>>
#define PAIR_VECTOR(id,value)		std::vector<std::pair<id,value>>
#define PAIR_MAP(key, value)		std::map<key,value,std::less<key>>
#define HASH_MAP(key, value)	std::unordered_map<key,value>