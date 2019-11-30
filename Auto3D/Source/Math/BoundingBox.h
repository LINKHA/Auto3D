#pragma once

#include "Rect.h"
#include "Vector3.h"

namespace Auto3D
{
template<typename _Ty> class TMatrix3x3;
template<typename _Ty> class TMatrix4x4;
template<typename _Ty> class TMatrix3x4;

class FPolyhedron;
class FFrustum;
class FSphere;

/// Three-dimensional axis-aligned bounding box.
template<typename _Ty> class AUTO_API TBoundingBox
{
public:
	/// Minimum vector.
	TVector3<_Ty> _min;
	/// Maximum vector.
	TVector3<_Ty> _max;

	/// Construct as undefined (negative size.)
	TBoundingBox() :
		_min(TVector3<_Ty>(M_INFINITY, M_INFINITY, M_INFINITY)),
		_max(TVector3<_Ty>(-M_INFINITY, -M_INFINITY, -M_INFINITY))
	{
	}

	/// Copy-construct.
	TBoundingBox(const TBoundingBox& box) :
		_min(box._min),
		_max(box._max)
	{
	}

	/// Construct from a rect, with the Z dimension left zero.
	TBoundingBox(const TRect<_Ty>& rect) :
		_min(TVector3<_Ty>(rect._min)),
		_max(TVector3<_Ty>(rect._max))
	{
	}

	/// Construct from minimum and maximum vectors.
	TBoundingBox(const TVector3<_Ty>& min, const TVector3<_Ty>& max) :
		_min(min),
		_max(max)
	{
	}

	/// Construct from minimum and maximum datas (all dimensions same.)
	TBoundingBox(_Ty min, _Ty max) :
		_min(TVector3<_Ty>(min, min, min)),
		_max(TVector3<_Ty>(max, max, max))
	{
	}

	/// Construct from an array of vertices.
	TBoundingBox(const TVector3<_Ty>* vertices, size_t count)
	{
		Define(vertices, count);
	}

	/// Construct from a frustum.
	TBoundingBox(const FFrustum& frustum)
	{
		Define(frustum);
	}

	/// Construct from a polyhedron.
	TBoundingBox(const FPolyhedron& poly)
	{
		Define(poly);
	}

	/// Construct from a sphere.
	TBoundingBox(const FSphere& sphere)
	{
		Define(sphere);
	}

	/// Construct by parsing a string.
	TBoundingBox(const FString& str)
	{
		FromString(str);
	}

	/// Construct by parsing a C string.
	TBoundingBox(const char* str)
	{
		FromString(str);
	}

	/// Assign from another bounding box.
	TBoundingBox& operator = (const TBoundingBox& rhs)
	{
		_min = rhs._min;
		_max = rhs._max;
		return *this;
	}

	/// Assign from a TRect, with the Z dimension left zero.
	TBoundingBox& operator = (const TRect<_Ty>& rhs)
	{
		_min = TVector3<_Ty>(rhs._min);
		_max = TVector3<_Ty>(rhs._max);
		return *this;
	}

	/// Test for equality with another bounding box without epsilon.
	bool operator == (const TBoundingBox& rhs) const { return _min == rhs._min && _max == rhs._max; }
	/// Test for inequality with another bounding box without epsilon.
	bool operator != (const TBoundingBox& rhs) const { return !(*this == rhs); }

	/// Define from another bounding box.
	void Define(const TBoundingBox& box)
	{
		_min = box._min;
		_max = box._max;
	}

	/// Define from a TRect.
	void Define(const TRect<_Ty>& rect)
	{
		_min = TVector3<_Ty>(rect._min);
		_max = TVector3<_Ty>(rect._max);
	}

	/// Define from minimum and maximum vectors.
	void Define(const TVector3<_Ty>& min, const TVector3<_Ty>& max)
	{
		_min = min;
		_max = max;
	}

	/// Define from minimum and maximum datas (all dimensions same.)
	void Define(_Ty min, _Ty max)
	{
		_min = TVector3<_Ty>(min, min, min);
		_max = TVector3<_Ty>(max, max, max);
	}

	/// Define from a point.
	void Define(const TVector3<_Ty>& point)
	{
		_min = _max = point;
	}

	/// Merge a point.
	void Merge(const TVector3<_Ty>& point)
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
	void Merge(const TBoundingBox& box)
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
		_min = TVector3<_Ty>(M_INFINITY, M_INFINITY, M_INFINITY);
		_max = -_min;
	}

	/// Define from an array of vertices.
	void Define(const TVector3<_Ty>* vertices, size_t count)
	{
		Undefine();
		Merge(vertices, count);
	}

	/// Define from a frustum.
	void Define(const FFrustum& frustum)
	{
		Define(frustum._vertices, NUM_FRUSTUM_VERTICES);
	}

	/// Define from a polyhedron.
	void Define(const FPolyhedron& poly)
	{
		Undefine();
		Merge(poly);
	}

	/// Define from a sphere.
	void Define(const FSphere& sphere)
	{
		const TVector3<_Ty>& center = sphere._center;
		_Ty radius = sphere._radius;

		_min = center + TVector3<_Ty>(-radius, -radius, -radius);
		_max = center + TVector3<_Ty>(radius, radius, radius);
	}

	/// Merge an array of vertices.
	void Merge(const TVector3<_Ty>* vertices, size_t count)
	{
		while (count--)
			Merge(*vertices++);
	}

	/// Merge a frustum.
	void Merge(const FFrustum& frustum)
	{
		Merge(frustum._vertices, NUM_FRUSTUM_VERTICES);
	}

	/// Merge a polyhedron.
	void Merge(const FPolyhedron& poly)
	{
		for (size_t i = 0; i < poly._faces.Size(); ++i)
		{
			const TVector<TVector3<_Ty>>& face = poly._faces[i];
			if (!face.IsEmpty())
				Merge(&face[0], face.Size());
		}
	}

	/// Merge a sphere.
	void Merge(const FSphere& sphere)
	{
		const TVector3<_Ty>& center = sphere._center;
		_Ty radius = sphere._radius;

		Merge(center + TVector3<_Ty>(radius, radius, radius));
		Merge(center + TVector3<_Ty>(-radius, -radius, -radius));
	}

	/// Clip with another bounding box.
	void Clip(const TBoundingBox& box)
	{
		if (box._min._x > _min._x)
			_min._x = box._min._x;
		if (box._max._x < _max._x)
			_max._x = box._max._x;
		if (box._min._y > _min._y)
			_min._y = box._min._y;
		if (box._max._y < _max._y)
			_max._y = box._max._y;
		if (box._min._z > _min._z)
			_min._z = box._min._z;
		if (box._max._z < _max._z)
			_max._z = box._max._z;

		if (_min._x > _max._x)
			Swap(_min._x, _max._x);
		if (_min._y > _max._y)
			Swap(_min._y, _max._y);
		if (_min._z > _max._z)
			Swap(_min._z, _max._z);
	}

	/// Transform with a 3x3 matrix.
	void Transform(const TMatrix3x3<_Ty>& transform)
	{
		*this = Transformed(TMatrix3x4<_Ty>(transform));
	}

	/// Transform with a 3x4 matrix.
	void Transform(const TMatrix3x4<_Ty>& transform)
	{
		*this = Transformed(transform);
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
		if (elements < 6)
			return false;

		char* ptr = (char*)str;
		_min._x = (_Ty)strtod(ptr, &ptr);
		_min._y = (_Ty)strtod(ptr, &ptr);
		_min._z = (_Ty)strtod(ptr, &ptr);
		_max._x = (_Ty)strtod(ptr, &ptr);
		_max._y = (_Ty)strtod(ptr, &ptr);
		_max._z = (_Ty)strtod(ptr, &ptr);

		return true;
	}

	/// Return whether has non-negative _size.
	bool IsDefined() const { return (_min._x <= _max._x); }
	/// Return center.
	TVector3<_Ty> Center() const { return (_max + _min) * 0.5f; }
	/// Return _size.
	TVector3<_Ty> Size() const { return _max - _min; }
	/// Return half-_size.
	TVector3<_Ty> HalfSize() const { return (_max - _min) * 0.5f; }
	/// Test for equality with another bounding box with epsilon.
	bool Equals(const TBoundingBox& box) const { return _min.Equals(box._min) && _max.Equals(box._max); }
	/// Return transformed by a 3x3 matrix.
	TBoundingBox Transformed(const TMatrix3x3<_Ty>& transform) const { return Transformed(TMatrix3x4<_Ty>(transform)); }
	/// Return transformed by a 3x4 matrix.
	TBoundingBox Transformed(const TMatrix3x4<_Ty>& transform) const
	{
		TVector3<_Ty> oldCenter = Center();
		TVector3<_Ty> oldEdge = _max - oldCenter;
		TVector3<_Ty> newCenter = transform * oldCenter;
		TVector3<_Ty> newEdge(
			Abs(transform._m00) * oldEdge._x + Abs(transform._m01) * oldEdge._y + Abs(transform._m02) * oldEdge._z,
			Abs(transform._m10) * oldEdge._x + Abs(transform._m11) * oldEdge._y + Abs(transform._m12) * oldEdge._z,
			Abs(transform._m20) * oldEdge._x + Abs(transform._m21) * oldEdge._y + Abs(transform._m22) * oldEdge._z
		);

		return TBoundingBox<_Ty>(newCenter - newEdge, newCenter + newEdge);
	}

	/// Return projected by a 4x4 projection matrix.
	TRect<_Ty> Projected(const TMatrix4x4<_Ty>& projection) const
	{
		TVector3<_Ty> projMin = _min;
		TVector3<_Ty> projMax = _max;
		if (projMin._z < M_EPSILON)
			projMin._z = M_EPSILON;
		if (projMax._z < M_EPSILON)
			projMax._z = M_EPSILON;

		TVector3<_Ty> vertices[8];
		vertices[0] = projMin;
		vertices[1] = TVector3<_Ty>(projMax._x, projMin._y, projMin._z);
		vertices[2] = TVector3<_Ty>(projMin._x, projMax._y, projMin._z);
		vertices[3] = TVector3<_Ty>(projMax._x, projMax._y, projMin._z);
		vertices[4] = TVector3<_Ty>(projMin._x, projMin._y, projMax._z);
		vertices[5] = TVector3<_Ty>(projMax._x, projMin._y, projMax._z);
		vertices[6] = TVector3<_Ty>(projMin._x, projMax._y, projMax._z);
		vertices[7] = projMax;

		TRect<_Ty> rect;
		for (size_t i = 0; i < 8; ++i)
		{
			TVector3<_Ty> projected = projection * vertices[i];
			rect.Merge(TVector2<_Ty>(projected._x, projected._y));
		}

		return rect;
	}

	/// Test if a point is inside.
	EIntersection::Type IsInside(const TVector3<_Ty>& point) const
	{
		if (point._x < _min._x || point._x > _max._x || point._y < _min._y || point._y > _max._y ||
			point._z < _min._z || point._z > _max._z)
			return EIntersection::OUTSIDE;
		else
			return EIntersection::INSIDE;
	}

	/// Test if another bounding box is inside, outside or intersects.
	EIntersection::Type IsInside(const TBoundingBox& box) const
	{
		if (box._max._x < _min._x || box._min._x > _max._x || box._max._y < _min._y || box._min._y > _max._y ||
			box._max._z < _min._z || box._min._z > _max._z)
			return EIntersection::OUTSIDE;
		else if (box._min._x < _min._x || box._max._x > _max._x || box._min._y < _min._y || box._max._y > _max._y ||
			box._min._z < _min._z || box._max._z > _max._z)
			return EIntersection::INTERSECTS;
		else
			return EIntersection::INSIDE;
	}

	/// Test if another bounding box is (partially) inside or outside.
	EIntersection::Type IsInsideFast(const TBoundingBox& box) const
	{
		if (box._max._x < _min._x || box._min._x > _max._x || box._max._y < _min._y || box._min._y > _max._y ||
			box._max._z < _min._z || box._min._z > _max._z)
			return EIntersection::OUTSIDE;
		else
			return EIntersection::INSIDE;
	}

	/// Test if a sphere is inside, outside or intersects.
	EIntersection::Type IsInside(const FSphere& sphere) const
	{
		_Ty distSquared = 0;
		_Ty temp;
		const TVector3<_Ty>& center = sphere._center;

		if (center._x < _min._x)
		{
			temp = center._x - _min._x;
			distSquared += temp * temp;
		}
		else if (center._x > _max._x)
		{
			temp = center._x - _max._x;
			distSquared += temp * temp;
		}
		if (center._y < _min._y)
		{
			temp = center._y - _min._y;
			distSquared += temp * temp;
		}
		else if (center._y > _max._y)
		{
			temp = center._y - _max._y;
			distSquared += temp * temp;
		}
		if (center._z < _min._z)
		{
			temp = center._z - _min._z;
			distSquared += temp * temp;
		}
		else if (center._z > _max._z)
		{
			temp = center._z - _max._z;
			distSquared += temp * temp;
		}

		_Ty radius = sphere._radius;
		if (distSquared >= radius * radius)
			return EIntersection::OUTSIDE;
		else if (center._x - radius < _min._x || center._x + radius > _max._x || center._y - radius < _min._y ||
			center._y + radius > _max._y || center._z - radius < _min._z || center._z + radius > _max._z)
			return EIntersection::INTERSECTS;
		else
			return EIntersection::INSIDE;
	}

	/// Test if a sphere is (partially) inside or outside.
	EIntersection::Type IsInsideFast(const FSphere& sphere) const
	{
		_Ty distSquared = 0;
		_Ty temp;
		const TVector3<_Ty>& center = sphere._center;

		if (center._x < _min._x)
		{
			temp = center._x - _min._x;
			distSquared += temp * temp;
		}
		else if (center._x > _max._x)
		{
			temp = center._x - _max._x;
			distSquared += temp * temp;
		}
		if (center._y < _min._y)
		{
			temp = center._y - _min._y;
			distSquared += temp * temp;
		}
		else if (center._y > _max._y)
		{
			temp = center._y - _max._y;
			distSquared += temp * temp;
		}
		if (center._z < _min._z)
		{
			temp = center._z - _min._z;
			distSquared += temp * temp;
		}
		else if (center._z > _max._z)
		{
			temp = center._z - _max._z;
			distSquared += temp * temp;
		}

		_Ty radius = sphere._radius;
		if (distSquared >= radius * radius)
			return EIntersection::OUTSIDE;
		else
			return EIntersection::INSIDE;
	}

	/// Return as string.
	FString ToString() const
	{
		return _min.ToString() + " " + _max.ToString();
	}
};

using TBoundingBoxF = TBoundingBox<float>;

using TBoundingBoxI = TBoundingBox<int>;

using TBoundingBoxC = TBoundingBox<char>;

using TBoundingBoxD = TBoundingBox<double>;

using TBoundingBoxU = TBoundingBox<unsigned>;

}
