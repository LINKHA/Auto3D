#pragma once

#include "AutoConfig.h"

namespace Auto3D
{

class FHashBase;
class FListBase;
class FVectorBase;
class FString;

/// Swap two values.
template<class _Ty> inline void Swap(_Ty& first, _Ty& second)
{
    _Ty temp = first;
    first = second;
    second = temp;
}

/// Swap two hash sets/maps.
template<> AUTO_API void Swap<FHashBase>(FHashBase& first, FHashBase& second);

/// Swap two lists.
template<> AUTO_API void Swap<FListBase>(FListBase& first, FListBase& second);

/// Swap two vectors.
template<> AUTO_API void Swap<FVectorBase>(FVectorBase& first, FVectorBase& second);

/// Swap two strings.
template<> AUTO_API void Swap<FString>(FString& first, FString& second);

}
