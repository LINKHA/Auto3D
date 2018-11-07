#pragma once
#include <list>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>


#define LIST(_T)					std::list<_T>
#define SET(_T)						std::set<_T,std::less<_T>>
#define VECTOR(_T)					std::vector<_T>
#define PAIR(_First,_Second)		std::pair(_First,_Second)
#define MAKE_PAIR(_First,_Second)	std::make_pair(_First,_Second)
#define PAIR_LIST(_Id,_T)			std::list<std::pair<_Id,_T>>
#define PAIR_SET(_Id,_T)			std::set<pair<_Id,_T>,std::less<pair<_Id,_T>>>
#define PAIR_VECTOR(_Id,_T)			std::vector<std::pair<_Id,_T>>
#define PAIR_MAP(_Kty, _T)			std::map<_Kty,_T,std::less<_Kty>>
#define HASH_MAP(_Kty, _T)			std::unordered_map<_Kty,_T>
#define STRING						std::string