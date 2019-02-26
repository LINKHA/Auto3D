#pragma once

#include "IntVector2.h"

namespace Auto3D
{

class String;

/// Two-dimensional bounding rectangle with integer values.
class AUTO_API IntRect
{
public:
    /// Left coordinate.
    int _left;
    /// Top coordinate.
    int _top;
    /// Right coordinate.
    int _right;
    /// Bottom coordinate.
    int _bottom;
    
    /// Construct undefined.
    IntRect()
    {
    }
    
    /// Copy-construct.
    IntRect(const IntRect& rect) :
        _left(rect._left),
        _top(rect._top),
        _right(rect._right),
        _bottom(rect._bottom)
    {
    }
    
    /// Construct from coordinates.
    IntRect(int left_, int top_, int right_, int bottom_) :
        _left(left_),
        _top(top_),
        _right(right_),
        _bottom(bottom_)
    {
    }
    
    /// Construct from an int array.
    IntRect(const int* data) :
        _left(data[0]),
        _top(data[1]),
        _right(data[2]),
        _bottom(data[3])
    {
    }

    /// Construct by parsing a string.
    IntRect(const String& str)
    {
        FromString(str);
    }
    
    /// Construct by parsing a C string.
    IntRect(const char* str)
    {
        FromString(str);
    }
    
    /// Test for equality with another rect.
    bool operator == (const IntRect& rhs) const { return _left == rhs._left && _top == rhs._top && _right == rhs._right && _bottom == rhs._bottom; }
    /// Test for inequality with another rect.
    bool operator != (const IntRect& rhs) const { return !(*this == rhs); }
    
    /// Parse from a string. Return true on success.
    bool FromString(const String& str);
    /// Parse from a C string. Return true on success.
    bool FromString(const char* str);
    
    /// Return _size.
    IntVector2 Size() const { return IntVector2(Width(), Height()); }
    /// Return width.
    int Width() const { return _right - _left; }
    /// Return height.
    int Height() const { return _bottom - _top; }
    
    /// Test whether a point is inside.
    Intersection IsInside(const IntVector2& point) const
    {
        if (point._x < _left || point._y < _top || point._x >= _right || point._y >= _bottom)
            return OUTSIDE;
        else
            return INSIDE;
    }

    /// Test whether another rect is inside or intersects.
    Intersection IsInside(const IntRect& rect) const
    {
        if (rect._right <= _left || rect._left >= _right || rect._bottom <= _top || rect._top >= _bottom)
            return OUTSIDE;
        else if (rect._left >= _left && rect._right <= _right && rect._top >= _top && rect._bottom <= _bottom)
            return INSIDE;
        else
            return INTERSECTS;
    }
    
    /// Return integer data.
    const int* Data() const { return &_left; }
    /// Return as string.
    String ToString() const;
    
    /// Zero-sized rect.
    static const IntRect ZERO;
};

}
