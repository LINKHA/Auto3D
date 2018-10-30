#pragma once

#include <KhSTL/KhSTL>

namespace Auto3D {

#define LIST(t)						KhSTL::List<t>
#define SET(t)						KhSTL::Set<t,KhSTL::Less<t>>
#define VECTOR(t)					KhSTL::Vector<t>
#define PAIR(t,x)					KhSTL::Pair(t,x)
#define MAKE_PAIR(t,x)				KhSTL::MakePair(t,x)
#define PAIR_LIST(id,value)			KhSTL::List<KhSTL::Pair<id,value>>
#define PAIR_SET(id,value)			KhSTL::Set<KhSTL::Pair<id,value>,KhSTL::Less<KhSTL::Pair<id,value>>>
#define PAIR_VECTOR(id,value)		KhSTL::Vector<KhSTL::Pair<id,value>>
#define PAIR_MAP(key, value)		KhSTL::Map<key,value,KhSTL::Less<KhSTL::tPair<key,value>>>
#define HASH_MAP(key, value)		KhSTL::UnorderedMap<key,value>
#define HASH_SET(key)				KhSTL::UnorderedSet<key>
#define STRING						KhSTL::String
#define TO_STRING(value)			KhSTL::ToString(value)

}