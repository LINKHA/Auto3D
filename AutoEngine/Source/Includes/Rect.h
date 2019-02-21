#pragma once
#include "Auto.h"
#include "MathBase.h"
#include "Vector2.h"
namespace Auto3D {
/// Two-dimensional bounding rectangle.
class Rect
{
public:
	/// Construct an undefined rect.
	Rect() noexcept :
		_min(M_INFINITY, M_INFINITY),
		_max(-M_INFINITY, -M_INFINITY)
	{
	}

	/// Construct from minimum and maximum vectors.
	Rect(const Vector2& min, const Vector2& max) noexcept :
		_min(min),
		_max(max)
	{
	}

	/// Construct from coordinates.
	Rect(float left, float top, float right, float bottom) noexcept :
		_min(left, top),
		_max(right, bottom)
	{
	}

	/// Construct from a Vector4.
	explicit Rect(const Vector4& vector) noexcept :
		_min(vector._x, vector._y),
		_max(vector._z, vector._w)
	{
	}

	/// Construct from a float array.
	explicit Rect(const float* data) noexcept :
		_min(data[0], data[1]),
		_max(data[2], data[3])
	{
	}

	/// Copy-construct from another rect.
	Rect(const Rect& rect) noexcept = default;

	/// Assign from another rect.
	Rect& operator =(const Rect& rhs) noexcept = default;

	/// Test for equality with another rect.
	bool operator ==(const Rect& rhs) const { return _min == rhs._min && _max == rhs._max; }

	/// Test for inequality with another rect.
	bool operator !=(const Rect& rhs) const { return _min != rhs._min || _max != rhs._max; }

	/// Add another rect to this one inplace.
	Rect& operator +=(const Rect& rhs)
	{
		_min += rhs._min;
		_max += rhs._max;
		return *this;
	}

	/// Subtract another rect from this one inplace.
	Rect& operator -=(const Rect& rhs)
	{
		_min -= rhs._min;
		_max -= rhs._max;
		return *this;
	}

	/// Divide by scalar inplace.
	Rect& operator /=(float value)
	{
		_min /= value;
		_max /= value;
		return *this;
	}

	/// Multiply by scalar inplace.
	Rect& operator *=(float value)
	{
		_min *= value;
		_max *= value;
		return *this;
	}

	/// Divide by scalar.
	Rect operator /(float value) const
	{
		return Rect(_min / value, _max / value);
	}

	/// Multiply by scalar.
	Rect operator *(float value) const
	{
		return Rect(_min * value, _max * value);
	}

	/// Add another rect.
	Rect operator +(const Rect& rhs) const
	{
		return Rect(_min + rhs._min, _max + rhs._max);
	}

	/// Subtract another rect.
	Rect operator -(const Rect& rhs) const
	{
		return Rect(_min - rhs._min, _max - rhs._max);
	}

	/// Define from another rect.
	void Define(const Rect& rect)
	{
		_min = rect._min;
		_max = rect._max;
	}

	/// Define from minimum and maximum vectors.
	void Define(const Vector2& min, const Vector2& max)
	{
		_min = min;
		_max = max;
	}

	/// Define from a point.
	void Define(const Vector2& point)
	{
		_min = _max = point;
	}

	/// Merge a point.
	void Merge(const Vector2& point)
	{
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
		if (rect._min._x < _min._x)
			_min._x = rect._min._x;
		if (rect._min._y < _min._y)
			_min._y = rect._min._y;
		if (rect._max._x > _max._x)
			_max._x = rect._max._x;
		if (rect._max._y > _max._y)
			_max._y = rect._max._y;
	}

	/// Clear to undefined state.
	void Clear()
	{
		_min = Vector2(M_INFINITY, M_INFINITY);
		_max = Vector2(-M_INFINITY, -M_INFINITY);
	}

	/// Clip with another rect.
	void Clip(const Rect& rect);

	/// Return true if this rect is defined via a previous call to Define() or Merge().
	bool Defined() const
	{
		return _min._x != M_INFINITY;
	}

	/// Return center.
	Vector2 Center() const { return (_max + _min) * 0.5f; }

	/// Return size.
	Vector2 Size() const { return _max - _min; }

	/// Return half-size.
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

	/// Test if another rect is inside, outside or intersects.
	Intersection IsInside(const Rect& rect) const
	{
		if (rect._max._x < _min._x || rect._min._x > _max._x || rect._max._y < _min._y || rect._min._y > _max._y)
			return OUTSIDE;
		else if (rect._min._x < _min._x || rect._max._x > _max._x || rect._min._y < _min._y || rect._max._y > _max._y)
			return INTERSECTS;
		else
			return INSIDE;
	}

	/// Return float data.
	const void* Data() const { return &_min._x; }

	/// Return as a vector.
	Vector4 ToVector4() const { return Vector4(_min._x, _min.y, _max._x, _max._y); }

	/// Return as string.
	STRING ToString() const;

	/// Return left-top corner position.
	Vector2 Min() const { return _min; }

	/// Return right-bottom corner position.
	Vector2 Max() const { return _max; }

	/// Return left coordinate.
	float Left() const { return _min._x; }

	/// Return top coordinate.
	float Top() const { return _min.y; }

	/// Return right coordinate.
	float Right() const { return _max._x; }

	/// Return bottom coordinate.
	float Bottom() const { return _max._y; }

	/// Minimum vector.
	Vector2 _min;
	/// Maximum vector.
	Vector2 _max;

	/// Rect in the range (-1, -1) - (1, 1)
	static const Rect FULL;
	/// Rect in the range (0, 0) - (1, 1)
	static const Rect POSITIVE;
	/// Zero-sized rect.
	static const Rect ZERO;
};

/// Two-dimensional bounding rectangle with integer values.
class RectInt
{
public:
	/// Construct a zero rect.
	RectInt() noexcept :
		_left(0),
		_top(0),
		_right(0),
		_bottom(0)
	{
	}

	/// Construct from minimum and maximum vectors.
	RectInt(const Vector2& min, const Vector2& max) noexcept :
		_left(min._x),
		_top(min._y),
		_right(max._x),
		_bottom(max.y)
	{
	}

	/// Construct from coordinates.
	RectInt(int left, int bottom, int right, int top) noexcept :
		_left(left),
		_top(top),
		_right(right),
		_bottom(bottom)
	{
	}

	/// Construct from an int array.
	explicit RectInt(const int* data) noexcept :
		_left(data[0]),
		_bottom(data[3]),
		_right(data[2]),
		_top(data[1])
	{
	}

	/// Test for equality with another rect.
	bool operator ==(const RectInt& rhs) const
	{
		return _left == rhs._left && _top == rhs._top && _right == rhs._right && _bottom == rhs._bottom;
	}

	/// Test for inequality with another rect.
	bool operator !=(const RectInt& rhs) const
	{
		return _left != rhs._left || _top != rhs._top || _right != rhs._right || _bottom != rhs._bottom;
	}

	/// Add another rect to this one inplace.
	RectInt& operator +=(const RectInt& rhs)
	{
		_left += rhs._left;
		_top += rhs._top;
		_right += rhs._right;
		_bottom += rhs._bottom;
		return *this;
	}

	/// Subtract another rect from this one inplace.
	RectInt& operator -=(const RectInt& rhs)
	{
		_left -= rhs._left;
		_top -= rhs._top;
		_right -= rhs._right;
		_bottom -= rhs._bottom;
		return *this;
	}

	/// Divide by scalar inplace.
	RectInt& operator /=(float value)
	{
		_left = static_cast<int>(_left / value);
		_top = static_cast<int>(_top / value);
		_right = static_cast<int>(_right / value);
		_bottom = static_cast<int>(_bottom / value);
		return *this;
	}

	/// Multiply by scalar inplace.
	RectInt& operator *=(float value)
	{
		_left = static_cast<int>(_left * value);
		_top = static_cast<int>(_top * value);
		_right = static_cast<int>(_right * value);
		_bottom = static_cast<int>(_bottom * value);
		return *this;
	}

	/// Divide by scalar.
	RectInt operator /(float value) const
	{
		return {
			static_cast<int>(_left / value), static_cast<int>(_top / value),
			static_cast<int>(_right / value), static_cast<int>(_bottom / value)
		};
	}

	/// Multiply by scalar.
	RectInt operator *(float value) const
	{
		return {
			static_cast<int>(_left * value), static_cast<int>(_top * value),
			static_cast<int>(_right * value), static_cast<int>(_bottom * value)
		};
	}

	/// Add another rect.
	RectInt operator +(const RectInt& rhs) const
	{
		return {
			_left + rhs._left, _top + rhs._top,
			_right + rhs._right, _bottom + rhs._bottom
		};
	}

	/// Subtract another rect.
	RectInt operator -(const RectInt& rhs) const
	{
		return {
			_left - rhs._left, _top - rhs._top,
			_right - rhs._right, _bottom - rhs._bottom
		};
	}

	/// Return size.
	Vector2 Size() const { return Vector2(Width(), Height()); }

	/// Return width.
	int Width() const { return _right - _left; }

	/// Return height.
	int Height() const { return  _top - _bottom; }

	/// Test whether a point is inside.
	Intersection IsInside(const Vector2& point) const
	{
		if (point._x < _left || point.y < _top || point._x >= _right || point._y >= _bottom)
			return OUTSIDE;
		else
			return INSIDE;
	}

	/// Clip with another rect.  Since IntRect does not have an undefined state
	/// like Rect, return (0, 0, 0, 0) if the result is empty.
	void Clip(const RectInt& rect);

	/// Merge a rect.  If this rect was empty, become the other rect.  If the
	/// other rect is empty, do nothing.
	void Merge(const RectInt& rect);

	/// Return integer data.
	const int* Data() const { return &_left; }

	/// Return as string.
	STRING ToString() const;

	/// Return left-top corner position.
	Vector2 Min() const { return { (float)_left, (float)_top }; }

	/// Return right-bottom corner position.
	Vector2 Max() const { return { (float)_right, (float)_bottom }; }

	/// Return left coordinate.
	int Left() const { return _left; }

	/// Return top coordinate.
	int Top() const { return _top; }

	/// Return right coordinate.
	int Right() const { return _right; }

	/// Return bottom coordinate.
	int Bottom() const { return _bottom; }

	/// Left coordinate.
	int _left;
	/// Top coordinate.
	int _top;
	/// Right coordinate.
	int _right;
	/// Bottom coordinate.
	int _bottom;

	/// Zero-sized rect.
	static const RectInt ZERO;
};

}

