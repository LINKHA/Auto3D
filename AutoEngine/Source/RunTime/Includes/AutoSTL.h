#pragma once
#include <list>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <string>

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
//
//template <typename _Ty>
//	using LIST = std::list<_Ty>;
//
//template <typename _Ty> 
//	using SET = std::set<_Ty, std::less<_Ty>>;
//
//template <typename _Ty> 
//	using VECTOR = std::vector<_Ty>;
//
//template <typename _First
//	, typename _Second> 
//	using PAIR = std::pair<_First, _Second>;
//
//template <typename _Id
//	, typename _Ty> 
//	using PAIR_LIST = std::list<std::pair<_Id, _Ty>>;
//
//template <typename _Id
//	, typename _Ty> 
//	using PAIR_SET = std::set<std::pair<_Id, _Ty>, std::less<std::pair<_Id, _Ty>>>;
//
//template <typename _Id
//	, typename _Ty> 
//	using PAIR_VECTOR = std::vector<std::pair<_Id, _Ty>>;
//
//template <typename _Kty
//	, typename _Ty> 
//	using PAIR_MAP = std::map<_Kty, _Ty, std::less<_Kty>>;
//
//template <typename _Kty
//	, typename _Ty> 
//	using HASH_MAP = std::unordered_map<_Kty, _Ty>;
//
//using STRING = std::string;
//
//#define MAKE_PAIR(_Firsy, _Second) = std::make_pair(_First, _Second)
