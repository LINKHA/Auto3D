#pragma once

#include "Vector4.h"
#include "Container/Swap.h"

namespace Auto3D
{

/// Two-dimensional bounding rectangle.
template <typename _Ty> class AUTO_API TRect
{
public:
    /// Minimum vector.
    TVector2<_Ty> _min;
    /// Maximum vector.
    TVector2<_Ty> _max;

    /// Construct as undefined (negative _size.)
    TRect() :
        _min(TVector2<_Ty>(M_INFINITY, M_INFINITY)),
        _max(TVector2<_Ty>(-M_INFINITY, -M_INFINITY))
    {
    }
    
    /// Copy-construct.
    TRect(const TRect& rect) :
        _min(rect._min),
        _max(rect._max)
    {
    }
    
    /// Construct from minimum and maximum vectors.
    TRect(const TVector2<_Ty>& min, const TVector2<_Ty>& max) :
        _min(min),
        _max(max)
    {
    }
    
    /// Construct from coordinates.
    TRect(float left, float top, float right, float bottom) :
        _min(left, top),
        _max(right, bottom)
    {
    }
    
    /// Construct from a TVector4.
    TRect(const TVector4<_Ty>& vector) :
        _min(vector._x, vector._y),
        _max(vector._z, vector._w)
    {
    }

    /// Construct from a float array.
    TRect(const float* _data) :
        _min(_data[0], _data[1]),
        _max(_data[2], _data[3])
    {
    }
    
    /// Construct by parsing a string.
    TRect(const FString& str)
    {
        FromString(str);
    }
    
    /// Construct by parsing a C string.
    TRect(const char* str)
    {
        FromString(str);
    }
    
    /// Assign from another rect.
    TRect& operator = (const TRect& rhs)
    {
        _min = rhs._min;
        _max = rhs._max;
		
        return *this;
    }
    
    /// Test for equality with another rect without epsilon.
    bool operator == (const TRect& rhs) const { return _min == rhs._min && _max == rhs._max; }
    /// Test for inequality with another rect without epsilon.
    bool operator != (const TRect& rhs) const { return !(*this == rhs); }
    
    /// Define from another rect.
    void Define(const TRect& rect)
    {
        _min = rect._min;
        _max = rect._max;
		
    }
    
    /// Define from minimum and maximum vectors.
    void Define(const TVector2<_Ty>& min_, const TVector2<_Ty>& max_)
    {
        _min = min_;
        _max = max_;
    }
    
    /// Define from a point.
    void Define(const TVector2<_Ty>& point)
    {
        _min = _max = point;
    }
    
    /// Merge a point.
    void Merge(const TVector2<_Ty>& point)
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
    void Merge(const TRect& rect)
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
        _min = TVector2<_Ty>(M_INFINITY, M_INFINITY);
        _max = -_min;
		
    }
    
    /// Clip with another rect.
	void Clip(const TRect& rect)
	{
		if (rect._min._x > _min._x)
			_min._x = rect._min._x;
		if (rect._max._x < _max._x)
			_max._x = rect._max._x;
		if (rect._min._y > _min._y)
			_min._y = rect._min._y;
		if (rect._max._y < _max._y)
			_max._y = rect._max._y;

		if (_min._x > _max._x)
			Swap(_min._x, _max._x);
		if (_min._y > _max._y)
			Swap(_min._y, _max._y);
		
	}
   /// Parse from a string. Return true on success.
	bool FromString(const FString& str)
	{
		return FromString(str.CString());
	}

    /// Parse from a C string. Return true on success.
	bool FromString(const char* str)
	{
		size_t elements = FString::CountElements(str, ' ');
		if (elements < 4)
			return false;

		char* ptr = (char*)str;
		_min._x = (float)strtod(ptr, &ptr);
		_min._y = (float)strtod(ptr, &ptr);
		_max._x = (float)strtod(ptr, &ptr);
		_max._y = (float)strtod(ptr, &ptr);

		
		return true;
	}
    
    /// Return whether has non-negative _size.
    bool IsDefined() const { return (_min._x <= _max._x); }
    /// Return center.
    TVector2<_Ty> Center() const { return (_max + _min) * 0.5f; }
    /// Return _size.
    TVector2<_Ty> Size() const { return _max - _min; }
    /// Return width.
	_Ty Width() const { return _max._x - _min._x; }
    /// Return height.
	_Ty Height() const { return _max._y - _min._y; }
	/// Return left
	_Ty& Left() { return _min._x; }
	/// Return top
	_Ty& Top() { return _min._y; }
	/// Return right
	_Ty& Right() { return _max._x; }
	/// Return bottom
	_Ty& Bottom() { return _max._y; }
	/// Return left
	const _Ty& Left() const { return _min._x; }
	/// Return top
	const _Ty& Top() const { return _min._y; }
	/// Return right
	const _Ty& Right() const { return _max._x; }
	/// Return bottom
	const _Ty& Bottom() const { return _max._y; }

    /// Return half-_size.
    TVector2<_Ty> HalfSize() const { return (_max - _min) * 0.5f; }
    /// Test for equality with another rect with epsilon.
    bool Equals(const TRect& rhs) const { return _min.Equals(rhs._min) && _max.Equals(rhs._max); }
    
    /// Test whether a point is inside with TVector2
    Intersection IsInside(const TVector2<_Ty>& point) const
    {
        if (point._x < _min._x || point._y < _min._y || point._x > _max._x || point._y > _max._y)
            return OUTSIDE;
        else
            return INSIDE;
    }
	/// Test whether another rect is inside with TRect
	Intersection IsInside(const TRect& rect) const
	{
		if (rect._max._x <= _min._x || rect._min._x >= _max._x || rect._max._y <= _min._y || rect._min._y >= _max._y)
			return OUTSIDE;
		else if (rect._min._x >= _min._x && rect._max._x <= _max._x && rect._min._y >= _min._y && rect._max._y <= _max._y)
			return INSIDE;
		else
			return INTERSECTS;
	}
    /// Return float data.
    const void* Data() const { return &_min._x; }
    /// Return as a vector.
    TVector4<_Ty> ToVector4() const { return TVector4<_Ty>(_min._x, _min._y, _max._x, _max._y); }
    /// Return as string.
	FString ToString() const
	{
		return _min.ToString() + " " + _max.ToString();
	}

    
    /// TRect in the range (-1, -1) - (1, 1)
    static const TRect FULL;
    /// TRect in the range (0, 0) - (1, 1)
    static const TRect POSITIVE;
    /// Zero-sized rect.
    static const TRect ZERO;
};

using TRectF = TRect<float>;

using TRectI = TRect<int>;

using TRectC = TRect<char>;

using TRectD = TRect<double>;

using TRectU = TRect<unsigned>;


}
