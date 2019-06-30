#pragma once

#include "../AutoConfig.h"

namespace Auto3D
{

class HashBase;
class ListBase;
class VectorBase;
class String;

/// Swap two values.
template<class _Ty> inline void Swap(_Ty& first, _Ty& second)
{
    _Ty temp = first;
    first = second;
    second = temp;
}

/// Swap two hash sets/maps.
template<> AUTO_API void Swap<HashBase>(HashBase& first, HashBase& second);

/// Swap two lists.
template<> AUTO_API void Swap<ListBase>(ListBase& first, ListBase& second);

/// Swap two vectors.
template<> AUTO_API void Swap<VectorBase>(VectorBase& first, VectorBase& second);

/// Swap two strings.
template<> AUTO_API void Swap<String>(String& first, String& second);

}
