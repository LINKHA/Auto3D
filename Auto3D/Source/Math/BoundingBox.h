#pragma once

#include "Rect.h"
#include "Vector3.h"

namespace Auto3D
{
template<typename _Ty> class Matrix3x3;
template<typename _Ty> class Matrix4x4;
template<typename _Ty> class Matrix3x4;

class Polyhedron;
class Frustum;
class Sphere;

/// Three-dimensional axis-aligned bounding box.
class AUTO_API BoundingBox
{
public:
	/// Minimum vector.
	Vector3<float> _min;
	/// Maximum vector.
	Vector3<float> _max;

	/// Construct as undefined (negative _size.)
	BoundingBox() :
		_min(Vector3<float>(M_INFINITY, M_INFINITY, M_INFINITY)),
		_max(Vector3<float>(-M_INFINITY, -M_INFINITY, -M_INFINITY))
	{
	}

	/// Copy-construct.
	BoundingBox(const BoundingBox& box) :
		_min(box._min),
		_max(box._max)
	{
	}

	/// Construct from a rect, with the Z dimension left zero.
	BoundingBox(const Rect<float>& rect) :
		_min(Vector3<float>(rect._min)),
		_max(Vector3<float>(rect._max))
	{
	}

	/// Construct from minimum and maximum vectors.
	BoundingBox(const Vector3<float>& min_, const Vector3<float>& max_) :
		_min(min_),
		_max(max_)
	{
	}

	/// Construct from minimum and maximum floats (all dimensions same.)
	BoundingBox(float min_, float max_) :
		_min(Vector3<float>(min_, min_, min_)),
		_max(Vector3<float>(max_, max_, max_))
	{
	}

	/// Construct from an array of vertices.
	BoundingBox(const Vector3<float>* vertices, size_t count)
	{
		Define(vertices, count);
	}

	/// Construct from a frustum.
	BoundingBox(const Frustum& frustum)
	{
		Define(frustum);
	}

	/// Construct from a polyhedron.
	BoundingBox(const Polyhedron& poly)
	{
		Define(poly);
	}

	/// Construct from a sphere.
	BoundingBox(const Sphere& sphere)
	{
		Define(sphere);
	}

	/// Construct by parsing a string.
	BoundingBox(const String& str)
	{
		FromString(str);
	}

	/// Construct by parsing a C string.
	BoundingBox(const char* str)
	{
		FromString(str);
	}

	/// Assign from another bounding box.
	BoundingBox& operator = (const BoundingBox& rhs)
	{
		_min = rhs._min;
		_max = rhs._max;
		return *this;
	}

	/// Assign from a Rect, with the Z dimension left zero.
	BoundingBox& operator = (const Rect<float>& rhs)
	{
		_min = Vector3<float>(rhs._min);
		_max = Vector3<float>(rhs._max);
		return *this;
	}

	/// Test for equality with another bounding box without epsilon.
	bool operator == (const BoundingBox& rhs) const { return _min == rhs._min && _max == rhs._max; }
	/// Test for inequality with another bounding box without epsilon.
	bool operator != (const BoundingBox& rhs) const { return !(*this == rhs); }

	/// Define from another bounding box.
	void Define(const BoundingBox& box)
	{
		_min = box._min;
		_max = box._max;
	}

	/// Define from a Rect.
	void Define(const Rect<float>& rect)
	{
		_min = Vector3<float>(rect._min);
		_max = Vector3<float>(rect._max);
	}

	/// Define from minimum and maximum vectors.
	void Define(const Vector3<float>& min, const Vector3<float>& max)
	{
		_min = min;
		_max = max;
	}

	/// Define from minimum and maximum floats (all dimensions same.)
	void Define(float min, float max)
	{
		_min = Vector3<float>(min, min, min);
		_max = Vector3<float>(max, max, max);
	}

	/// Define from a point.
	void Define(const Vector3<float>& point)
	{
		_min = _max = point;
	}

	/// Merge a point.
	void Merge(const Vector3<float>& point)
	{
		// If undefined, set initial dimensions
		if (!IsDefined())
		{
			_min = _max = point;
			return;
		}

		if (point._x < _min._x)
			_min._x = point._x;
		if (point._y < _min._y)
			_min._y = point._y;
		if (point._z < _min._z)
			_min._z = point._z;
		if (point._x > _max._x)
			_max._x = point._x;
		if (point._y > _max._y)
			_max._y = point._y;
		if (point._z > _max._z)
			_max._z = point._z;
	}

	/// Merge another bounding box.
	void Merge(const BoundingBox& box)
	{
		if (_min._x > _max._x)
		{
			_min = box._min;
			_max = box._max;
			return;
		}

		if (box._min._x < _min._x)
			_min._x = box._min._x;
		if (box._min._y < _min._y)
			_min._y = box._min._y;
		if (box._min._z < _min._z)
			_min._z = box._min._z;
		if (box._max._x > _max._x)
			_max._x = box._max._x;
		if (box._max._y > _max._y)
			_max._y = box._max._y;
		if (box._max._z > _max._z)
			_max._z = box._max._z;
	}

	/// Set as undefined (negative _size) to allow the next merge to set initial _size.
	void Undefine()
	{
		_min = Vector3<float>(M_INFINITY, M_INFINITY, M_INFINITY);
		_max = -_min;
	}

	/// Define from an array of vertices.
	void Define(const Vector3<float>* vertices, size_t count);
	/// Define from a frustum.
	void Define(const Frustum& frustum);
	/// Define from a polyhedron.
	void Define(const Polyhedron& poly);
	/// Define from a sphere.
	void Define(const Sphere& sphere);
	/// Merge an array of vertices.
	void Merge(const Vector3<float>* vertices, size_t count);
	/// Merge a frustum.
	void Merge(const Frustum& frustum);
	/// Merge a polyhedron.
	void Merge(const Polyhedron& poly);
	/// Merge a sphere.
	void Merge(const Sphere& sphere);
	/// Clip with another bounding box.
	void Clip(const BoundingBox& box);
	/// Transform with a 3x3 matrix.
	void Transform(const Matrix3x3<float>& transform);
	/// Transform with a 3x4 matrix.
	void Transform(const Matrix3x4<float>& transform);
	/// Parse from a string. Return true on success.
	bool FromString(const String& str);
	/// Parse from a C string. Return true on success.
	bool FromString(const char* str);

	/// Return whether has non-negative _size.
	bool IsDefined() const { return (_min._x <= _max._x); }
	/// Return center.
	Vector3<float> Center() const { return (_max + _min) * 0.5f; }
	/// Return _size.
	Vector3<float> Size() const { return _max - _min; }
	/// Return half-_size.
	Vector3<float> HalfSize() const { return (_max - _min) * 0.5f; }
	/// Test for equality with another bounding box with epsilon.
	bool Equals(const BoundingBox& box) const { return _min.Equals(box._min) && _max.Equals(box._max); }

	/// Return transformed by a 3x3 matrix.
	BoundingBox Transformed(const Matrix3x3<float>& transform) const;
	/// Return transformed by a 3x4 matrix.
	BoundingBox Transformed(const Matrix3x4<float>& transform) const;
	/// Return projected by a 4x4 projection matrix.
	Rect<float> Projected(const Matrix4x4<float>& projection) const;

	/// Test if a point is inside.
	Intersection IsInside(const Vector3<float>& point) const
	{
		if (point._x < _min._x || point._x > _max._x || point._y < _min._y || point._y > _max._y ||
			point._z < _min._z || point._z > _max._z)
			return OUTSIDE;
		else
			return INSIDE;
	}

	/// Test if another bounding box is inside, outside or intersects.
	Intersection IsInside(const BoundingBox& box) const
	{
		if (box._max._x < _min._x || box._min._x > _max._x || box._max._y < _min._y || box._min._y > _max._y ||
			box._max._z < _min._z || box._min._z > _max._z)
			return OUTSIDE;
		else if (box._min._x < _min._x || box._max._x > _max._x || box._min._y < _min._y || box._max._y > _max._y ||
			box._min._z < _min._z || box._max._z > _max._z)
			return INTERSECTS;
		else
			return INSIDE;
	}

	/// Test if another bounding box is (partially) inside or outside.
	Intersection IsInsideFast(const BoundingBox& box) const
	{
		if (box._max._x < _min._x || box._min._x > _max._x || box._max._y < _min._y || box._min._y > _max._y ||
			box._max._z < _min._z || box._min._z > _max._z)
			return OUTSIDE;
		else
			return INSIDE;
	}

	/// Test if a sphere is inside, outside or intersects.
	Intersection IsInside(const Sphere& sphere) const;
	/// Test if a sphere is (partially) inside or outside.
	Intersection IsInsideFast(const Sphere& sphere) const;

	/// Return as string.
	String ToString() const;
};

}
