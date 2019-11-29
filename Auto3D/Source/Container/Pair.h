#pragma once

#include "Hash.h"

namespace Auto3D
{

/// %TPair template class.
template <typename _Ty1, typename _Ty2> class TPair
{
public:
    /// Construct undefined.
    TPair()
    {
    }
    
    /// Construct with values.
    TPair(const _Ty1& first_, const _Ty2& second_) :
        _first(first_),
        _second(second_)
    {
    }
    
    /// Test for equality with another pair.
    bool operator == (const TPair<_Ty1, _Ty2>& rhs) const { return _first == rhs._first && _second == rhs._second; }
    /// Test for inequality with another pair.
    bool operator != (const TPair<_Ty1, _Ty2>& rhs) const { return !(*this == rhs); }
    
    /// Test for less than with another pair.
    bool operator < (const TPair<_Ty1, _Ty2>& rhs) const
    {
        if (_first < rhs._first)
            return true;
        if (_first != rhs._first)
            return false;
        return _second < rhs._second;
    }
    
    /// Test for greater than with another pair.
    bool operator > (const TPair<_Ty1, _Ty2>& rhs) const
    {
        if (_first > rhs._first)
            return true;
        if (_first != rhs._first)
            return false;
        return _second > rhs._second;
    }
    
    /// Return hash value for THashSet & THashMap.
    unsigned ToHash() const { return (MakeHash(_first) & 0xffff) | (MakeHash(_second) << 16); }
    
    /// First value.
	_Ty1 _first;
    /// Second value.
    _Ty2 _second;
};

/// Construct a pair.
template <typename _Ty1, typename _Ty2> TPair<_Ty1, _Ty2> MakePair(const _Ty1& first, const _Ty2& second)
{
    return TPair<_Ty1, _Ty2>(first, second);
}

}
