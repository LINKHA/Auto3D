#pragma once

#include <list>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <Boost/smart_ptr.hpp>

#include <AutoString.h>


namespace Auto3D {



template <typename _Ty>
using LIST = std::list<_Ty>;

template <typename _Ty>
using SET = std::set<_Ty, std::less<_Ty> >;

template <typename _Ty>
using VECTOR = std::vector<_Ty>;

template <typename _First, typename _Second>
using PAIR = std::pair<_First, _Second>;

template <typename _Id, typename _Ty>
using PAIR_LIST = std::list<std::pair<_Id, _Ty> >;

template <typename _Id, typename _Ty>
using PAIR_SET = std::set<std::pair<_Id, _Ty>, std::less<std::pair<_Id, _Ty> > >;

template <typename _Id, typename _Ty>
using PAIR_VECTOR = std::vector<std::pair<_Id, _Ty> >;

template <typename _Kty, typename _Ty>
using PAIR_MAP = std::map<_Kty, _Ty, std::less<_Kty> >;

template <typename _Kty>
using HASH_SET = std::unordered_set<_Kty>;

template <typename _Kty, typename _Ty>
using HASH_MAP = std::unordered_map<_Kty, _Ty>;

template<typename _First,typename _Second> 
auto MakePair(_First rhs, _Second lfs) { return std::make_pair(rhs, lfs); }

template<typename _Ty> using SharedPtr = boost::shared_ptr<_Ty>;

template<typename _Ty> using WeakPtr = boost::weak_ptr<_Ty>;

template<typename _Ty> using SharedArrayPtr = boost::shared_array<_Ty>;

template<typename _Ty, typename... _Args>  
auto StaticCast(_Args&&... args) { return boost::static_pointer_cast<_Ty>(args...); }

template<typename _Ty, typename... _Args>
auto DynamicCast(_Args&&... args) { return boost::dynamic_pointer_cast<_Ty>(args...); }

template<typename _Ty, typename... _Args>
auto ConstCast(_Args&&... args) { return boost::const_pointer_cast<_Ty>(args...); }

template<typename _Ty, typename... _Args>
auto ReinterpretCast(_Args&&... args) { return boost::reinterpret_pointer_cast<_Ty>(args...); }

template<typename _Ty, typename... _Args> 
auto MakeShared(_Args&&... args) { return boost::make_shared<_Ty>(args...); }

}