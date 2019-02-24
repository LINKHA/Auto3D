// For conditions of distribution and use, see copyright notice in License.txt

#pragma once

#include "Rect.h"
#include "Vector3.h"
#undef min
#undef max

namespace Auto3D
{

template<typename _Ty> class Matrix3x3;
template<typename _Ty> class Matrix4x4;
template<typename _Ty> class Matrix3x4;
class Polyhedron;
class Frustum;
class Sphere;

/// Three-dimensional axis-aligned bounding box.
class BoundingBox
{
public:
	/// Minimum vector.
	Vector3F min;
	/// Maximum vector.
	Vector3F max;

	/// Construct as undefined (negative size.)
	BoundingBox() :
		min(Vector3F(M_INFINITY, M_INFINITY, M_INFINITY)),
		max(Vector3F(-M_INFINITY, -M_INFINITY, -M_INFINITY))
	{
	}

	/// Copy-construct.
	BoundingBox(const BoundingBox& box) :
		min(box.min),
		max(box.max)
	{
	}

	/// Construct from a rect, with the Z dimension left zero.
	BoundingBox(const Rect& rect) :
		min(Vector3F(rect._min)),
		max(Vector3F(rect._max))
	{
	}

	/// Construct from minimum and maximum vectors.
	BoundingBox(const Vector3F& min_, const Vector3F& max_) :
		min(min_),
		max(max_)
	{
	}

	/// Construct from minimum and maximum floats (all dimensions same.)
	BoundingBox(float min_, float max_) :
		min(Vector3F(min_, min_, min_)),
		max(Vector3F(max_, max_, max_))
	{
	}

	/// Construct from an array of vertices.
	BoundingBox(const Vector3F* vertices, size_t count)
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
	BoundingBox(const STRING& str)
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
		min = rhs.min;
		max = rhs.max;
		return *this;
	}

	/// Assign from a Rect, with the Z dimension left zero.
	BoundingBox& operator = (const Rect& rhs)
	{
		min = Vector3F(rhs._min);
		max = Vector3F(rhs._max);
		return *this;
	}

	/// Test for equality with another bounding box without epsilon.
	bool operator == (const BoundingBox& rhs) const { return min == rhs.min && max == rhs.max; }
	/// Test for inequality with another bounding box without epsilon.
	bool operator != (const BoundingBox& rhs) const { return !(*this == rhs); }

	/// Define from another bounding box.
	void Define(const BoundingBox& box)
	{
		min = box.min;
		max = box.max;
	}

	/// Define from a Rect.
	void Define(const Rect& rect)
	{
		min = Vector3F(rect._min);
		max = Vector3F(rect._max);
	}

	/// Define from minimum and maximum vectors.
	void Define(const Vector3F& min_, const Vector3F& max_)
	{
		min = min_;
		max = max_;
	}

	/// Define from minimum and maximum floats (all dimensions same.)
	void Define(float min_, float max_)
	{
		min = Vector3F(min_, min_, min_);
		max = Vector3F(max_, max_, max_);
	}

	/// Define from a point.
	void Define(const Vector3F& point)
	{
		min = max = point;
	}

	/// Merge a point.
	void Merge(const Vector3F& point)
	{
		// If undefined, set initial dimensions
		if (!IsDefined())
		{
			min = max = point;
			return;
		}

		if (point._x < min._x)
			min._x = point._x;
		if (point._y < min._y)
			min._y = point._y;
		if (point._z < min._z)
			min._z = point._z;
		if (point._x > max._x)
			max._x = point._x;
		if (point._y > max._y)
			max._y = point._y;
		if (point._z > max._z)
			max._z = point._z;
	}

	/// Merge another bounding box.
	void Merge(const BoundingBox& box)
	{
		if (min._x > max._x)
		{
			min = box.min;
			max = box.max;
			return;
		}

		if (box.min._x < min._x)
			min._x = box.min._x;
		if (box.min._y < min._y)
			min._y = box.min._y;
		if (box.min._z < min._z)
			min._z = box.min._z;
		if (box.max._x > max._x)
			max._x = box.max._x;
		if (box.max._y > max._y)
			max._y = box.max._y;
		if (box.max._z > max._z)
			max._z = box.max._z;
	}

	/// Set as undefined (negative size) to allow the next merge to set initial size.
	void Undefine()
	{
		min = Vector3F(M_INFINITY, M_INFINITY, M_INFINITY);
		max = -min;
	}

	/// Define from an array of vertices.
	void Define(const Vector3F* vertices, size_t count);
	/// Define from a frustum.
	void Define(const Frustum& frustum);
	/// Define from a polyhedron.
	void Define(const Polyhedron& poly);
	/// Define from a sphere.
	void Define(const Sphere& sphere);
	/// Merge an array of vertices.
	void Merge(const Vector3F* vertices, size_t count);
	/// Merge a frustum.
	void Merge(const Frustum& frustum);
	/// Merge a polyhedron.
	void Merge(const Polyhedron& poly);
	/// Merge a sphere.
	void Merge(const Sphere& sphere);
	/// Clip with another bounding box.
	void Clip(const BoundingBox& box);
	/// Transform with a 3x3 matrix.
	void Transform(const Matrix3x3F& transform);
	/// Transform with a 3x4 matrix.
	void Transform(const Matrix3x4F& transform);
	/// Parse from a string. Return true on success.
	bool FromString(const STRING& str);
	/// Parse from a C string. Return true on success.
	bool FromString(const char* str);

	/// Return whether has non-negative size.
	bool IsDefined() const { return (min._x <= max._x); }
	/// Return center.
	Vector3F Center() const { return (max + min) * 0.5f; }
	/// Return size.
	Vector3F Size() const { return max - min; }
	/// Return half-size.
	Vector3F HalfSize() const { return (max - min) * 0.5f; }
	/// Test for equality with another bounding box with epsilon.
	bool Equals(const BoundingBox& box) const { return min.Equals(box.min) && max.Equals(box.max); }

	/// Return transformed by a 3x3 matrix.
	BoundingBox Transformed(const Matrix3x3F& transform) const;
	/// Return transformed by a 3x4 matrix.
	BoundingBox Transformed(const Matrix3x4F& transform) const;
	/// Return projected by a 4x4 projection matrix.
	Rect Projected(const Matrix4x4F& projection) const;

	/// Test if a point is inside.
	Intersection IsInside(const Vector3F& point) const
	{
		if (point._x < min._x || point._x > max._x || point._y < min._y || point._y > max._y ||
			point._z < min._z || point._z > max._z)
			return OUTSIDE;
		else
			return INSIDE;
	}

	/// Test if another bounding box is inside, outside or intersects.
	Intersection IsInside(const BoundingBox& box) const
	{
		if (box.max._x < min._x || box.min._x > max._x || box.max._y < min._y || box.min._y > max._y ||
			box.max._z < min._z || box.min._z > max._z)
			return OUTSIDE;
		else if (box.min._x < min._x || box.max._x > max._x || box.min._y < min._y || box.max._y > max._y ||
			box.min._z < min._z || box.max._z > max._z)
			return INTERSECTS;
		else
			return INSIDE;
	}

	/// Test if another bounding box is (partially) inside or outside.
	Intersection IsInsideFast(const BoundingBox& box) const
	{
		if (box.max._x < min._x || box.min._x > max._x || box.max._y < min._y || box.min._y > max._y ||
			box.max._z < min._z || box.min._z > max._z)
			return OUTSIDE;
		else
			return INSIDE;
	}

	/// Test if a sphere is inside, outside or intersects.
	Intersection IsInside(const Sphere& sphere) const;
	/// Test if a sphere is (partially) inside or outside.
	Intersection IsInsideFast(const Sphere& sphere) const;

	/// Return as string.
	STRING ToString() const;
};

}
