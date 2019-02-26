#pragma once

#include "Vector4.h"

namespace Auto3D
{

/// Two-dimensional bounding rectangle.
class AUTO_API Rect
{
public:
    /// Minimum vector.
    Vector2 _min;
    /// Maximum vector.
    Vector2 _max;
    
    /// Construct as undefined (negative _size.)
    Rect() :
        _min(Vector2(M_INFINITY, M_INFINITY)),
        _max(Vector2(-M_INFINITY, -M_INFINITY))
    {
    }
    
    /// Copy-construct.
    Rect(const Rect& rect) :
        _min(rect._min),
        _max(rect._max)
    {
    }
    
    /// Construct from minimum and maximum vectors.
    Rect(const Vector2& min_, const Vector2& max_) :
        _min(min_),
        _max(max_)
    {
    }
    
    /// Construct from coordinates.
    Rect(float left, float top, float right, float bottom) :
        _min(left, top),
        _max(right, bottom)
    {
    }
    
    /// Construct from a Vector4.
    Rect(const Vector4& vector) :
        _min(vector._x, vector._y),
        _max(vector._z, vector._w)
    {
    }

    /// Construct from a float array.
    Rect(const float* data) :
        _min(data[0], data[1]),
        _max(data[2], data[3])
    {
    }
    
    /// Construct by parsing a string.
    Rect(const String& str)
    {
        FromString(str);
    }
    
    /// Construct by parsing a C string.
    Rect(const char* str)
    {
        FromString(str);
    }
    
    /// Assign from another rect.
    Rect& operator = (const Rect& rhs)
    {
        _min = rhs._min;
        _max = rhs._max;
        return *this;
    }
    
    /// Test for equality with another rect without epsilon.
    bool operator == (const Rect& rhs) const { return _min == rhs._min && _max == rhs._max; }
    /// Test for inequality with another rect without epsilon.
    bool operator != (const Rect& rhs) const { return !(*this == rhs); }
    
    /// Define from another rect.
    void Define(const Rect& rect)
    {
        _min = rect._min;
        _max = rect._max;
    }
    
    /// Define from minimum and maximum vectors.
    void Define(const Vector2& min_, const Vector2& max_)
    {
        _min = min_;
        _max = max_;
    }
    
    /// Define from a point.
    void Define(const Vector2& point)
    {
        _min = _max = point;
    }
    
    /// Merge a point.
    void Merge(const Vector2& point)
    {
        // If undefined, set initial dimensions
        if (!IsDefined())
        {
            _min = _max = point;
            return;
        }
        
        if (point._x < _min._x)
            _min._x = point._x;
        if (point._x > _max._x)
            _max._x = point._x;
        if (point._y < _min._y)
            _min._y = point._y;
        if (point._y > _max._y)
            _max._y = point._y;
    }
    
    /// Merge a rect.
    void Merge(const Rect& rect)
    {
       if (_min._x > _max._x)
        {
            _min = rect._min;
            _max = rect._max;
            return;
        }
        
        if (rect._min._x < _min._x)
            _min._x = rect._min._x;
        if (rect._min._y < _min._y)
            _min._y = rect._min._y;
        if (rect._max._x > _max._x)
            _max._x = rect._max._x;
        if (rect._max._y > _max._y)
            _max._y = rect._max._y;
    }
    
    /// Set as undefined to allow the next merge to set initial _size.
    void Undefine()
    {
        _min = Vector2(M_INFINITY, M_INFINITY);
        _max = -_min;
    }
    
    /// Clip with another rect.
    void Clip(const Rect& rect);
   /// Parse from a string. Return true on success.
    bool FromString(const String& str);
    /// Parse from a C string. Return true on success.
    bool FromString(const char* str);
    
    /// Return whether has non-negative _size.
    bool IsDefined() const { return (_min._x <= _max._x); }
    /// Return center.
    Vector2 Center() const { return (_max + _min) * 0.5f; }
    /// Return _size.
    Vector2 Size() const { return _max - _min; }
    /// Return half-_size.
    Vector2 HalfSize() const { return (_max - _min) * 0.5f; }
    /// Test for equality with another rect with epsilon.
    bool Equals(const Rect& rhs) const { return _min.Equals(rhs._min) && _max.Equals(rhs._max); }
    
    /// Test whether a point is inside.
    Intersection IsInside(const Vector2& point) const
    {
        if (point._x < _min._x || point._y < _min._y || point._x > _max._x || point._y > _max._y)
            return OUTSIDE;
        else
            return INSIDE;
    }
    
    /// Return float data.
    const void* Data() const { return &_min._x; }
    /// Return as a vector.
    Vector4 ToVector4() const { return Vector4(_min._x, _min._y, _max._x, _max._y); }
    /// Return as string.
    String ToString() const;
    
    /// Rect in the range (-1, -1) - (1, 1)
    static const Rect FULL;
    /// Rect in the range (0, 0) - (1, 1)
    static const Rect POSITIVE;
    /// Zero-sized rect.
    static const Rect ZERO;
};

}
