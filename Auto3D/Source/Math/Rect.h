#pragma once

#include "Vector4.h"
#include "../Base/Swap.h"

namespace Auto3D
{

/// Two-dimensional bounding rectangle with integer values.
class AUTO_API BaseRect
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
	BaseRect()
	{
	}

	/// Copy-construct.
	BaseRect(const BaseRect& rect) :
		_left(rect._left),
		_top(rect._top),
		_right(rect._right),
		_bottom(rect._bottom)
	{
	}

	/// Construct from coordinates.
	BaseRect(int left_, int top_, int right_, int bottom_) :
		_left(left_),
		_top(top_),
		_right(right_),
		_bottom(bottom_)
	{
	}

	/// Construct from an int array.
	BaseRect(const int* data) :
		_left(data[0]),
		_top(data[1]),
		_right(data[2]),
		_bottom(data[3])
	{
	}

	/// Construct by parsing a string.
	BaseRect(const String& str)
	{
		FromString(str);
	}

	/// Construct by parsing a C string.
	BaseRect(const char* str)
	{
		FromString(str);
	}

	/// Test for equality with another rect.
	bool operator == (const BaseRect& rhs) const { return _left == rhs._left && _top == rhs._top && _right == rhs._right && _bottom == rhs._bottom; }
	/// Test for inequality with another rect.
	bool operator != (const BaseRect& rhs) const { return !(*this == rhs); }

	/// Parse from a string. Return true on success.
	bool FromString(const String& str)
	{
		return FromString(str.CString());
	}
	/// Parse from a C string. Return true on success.
	bool FromString(const char* str)
	{
		size_t elements = String::CountElements(str, ' ');
		if (elements < 4)
			return false;

		char* ptr = (char*)str;
		_left = strtol(ptr, &ptr, 10);
		_top = strtol(ptr, &ptr, 10);
		_right = strtol(ptr, &ptr, 10);
		_bottom = strtol(ptr, &ptr, 10);

		return true;
	}

	/// Return _size.
	Vector2I Size() const { return Vector2I(Width(), Height()); }
	/// Return width.
	int Width() const { return _right - _left; }
	/// Return height.
	int Height() const { return _bottom - _top; }

	/// Test whether a point is inside.
	Intersection IsInside(const Vector2I& point) const
	{
		if (point._x < _left || point._y < _top || point._x >= _right || point._y >= _bottom)
			return OUTSIDE;
		else
			return INSIDE;
	}

	/// Test whether another rect is inside or intersects.
	Intersection IsInside(const BaseRect& rect) const
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
	String ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%d %d %d %d", _left, _top, _right, _bottom);
		return String(tempBuffer);
	}

	/// Zero-sized rect.
	static const BaseRect ZERO;
};

/// Two-dimensional bounding rectangle.
template <typename _Ty> class AUTO_API Rect
{
public:
    /// Minimum vector.
    Vector2<_Ty> _min;
    /// Maximum vector.
    Vector2<_Ty> _max;

    /// Construct as undefined (negative _size.)
    Rect() :
        _min(Vector2<_Ty>(M_INFINITY, M_INFINITY)),
        _max(Vector2<_Ty>(-M_INFINITY, -M_INFINITY))
    {
    }
    
    /// Copy-construct.
    Rect(const Rect& rect) :
        _min(rect._min),
        _max(rect._max)
    {
    }
    
    /// Construct from minimum and maximum vectors.
    Rect(const Vector2<_Ty>& min, const Vector2<_Ty>& max) :
        _min(min),
        _max(max)
    {
    }
    
    /// Construct from coordinates.
    Rect(float left, float top, float right, float bottom) :
        _min(left, top),
        _max(right, bottom)
    {
    }
    
    /// Construct from a Vector4.
    Rect(const Vector4<_Ty>& vector) :
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
    void Define(const Vector2<_Ty>& min_, const Vector2<_Ty>& max_)
    {
        _min = min_;
        _max = max_;
    }
    
    /// Define from a point.
    void Define(const Vector2<_Ty>& point)
    {
        _min = _max = point;
    }
    
    /// Merge a point.
    void Merge(const Vector2<_Ty>& point)
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
        _min = Vector2<_Ty>(M_INFINITY, M_INFINITY);
        _max = -_min;
		
    }
    
    /// Clip with another rect.
	void Clip(const Rect& rect)
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
	bool FromString(const String& str)
	{
		return FromString(str.CString());
	}

    /// Parse from a C string. Return true on success.
	bool FromString(const char* str)
	{
		size_t elements = String::CountElements(str, ' ');
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
    Vector2<_Ty> Center() const { return (_max + _min) * 0.5f; }
    /// Return _size.
    Vector2<_Ty> Size() const { return _max - _min; }
    /// Return width.
	_Ty Width() const { return _max._x - _min._y; }
    /// Return height.
	_Ty Height() const { return _max._y - _min._x; }
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
    Vector2<_Ty> HalfSize() const { return (_max - _min) * 0.5f; }
    /// Test for equality with another rect with epsilon.
    bool Equals(const Rect& rhs) const { return _min.Equals(rhs._min) && _max.Equals(rhs._max); }
    
    /// Test whether a point is inside with Vector2
    Intersection IsInside(const Vector2<_Ty>& point) const
    {
        if (point._x < _min._x || point._y < _min._y || point._x > _max._x || point._y > _max._y)
            return OUTSIDE;
        else
            return INSIDE;
    }
	/// Test whether another rect is inside with Rect
	Intersection IsInside(const Rect& rect) const
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
    Vector4<_Ty> ToVector4() const { return Vector4<_Ty>(_min._x, _min._y, _max._x, _max._y); }
    /// Return as string.
	String ToString() const
	{
		return _min.ToString() + " " + _max.ToString();
	}

    
    /// Rect in the range (-1, -1) - (1, 1)
    static const Rect FULL;
    /// Rect in the range (0, 0) - (1, 1)
    static const Rect POSITIVE;
    /// Zero-sized rect.
    static const Rect ZERO;
};

using RectF = Rect<float>;

using RectI = Rect<int>;

using RectC = Rect<char>;

using RectD = Rect<double>;

using RectU = Rect<unsigned>;


}
