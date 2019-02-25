#pragma once

#include "Hash.h"

namespace Auto3D
{

/// %Pair template class.
template <class _Ty, class U> class Pair
{
public:
    /// Construct undefined.
    Pair()
    {
    }
    
    /// Construct with values.
    Pair(const _Ty& first_, const U& second_) :
        first(first_),
        second(second_)
    {
    }
    
    /// Test for equality with another pair.
    bool operator == (const Pair<_Ty, U>& rhs) const { return first == rhs.first && second == rhs.second; }
    /// Test for inequality with another pair.
    bool operator != (const Pair<_Ty, U>& rhs) const { return !(*this == rhs); }
    
    /// Test for less than with another pair.
    bool operator < (const Pair<_Ty, U>& rhs) const
    {
        if (first < rhs.first)
            return true;
        if (first != rhs.first)
            return false;
        return second < rhs.second;
    }
    
    /// Test for greater than with another pair.
    bool operator > (const Pair<_Ty, U>& rhs) const
    {
        if (first > rhs.first)
            return true;
        if (first != rhs.first)
            return false;
        return second > rhs.second;
    }
    
    /// Return hash value for HashSet & HashMap.
    unsigned ToHash() const { return (MakeHash(first) & 0xffff) | (MakeHash(second) << 16); }
    
    /// First value.
    _Ty first;
    /// Second value.
    U second;
};

/// Construct a pair.
template <class _Ty, class U> Pair<_Ty, U> MakePair(const _Ty& first, const U& second)
{
    return Pair<_Ty, U>(first, second);
}

}
