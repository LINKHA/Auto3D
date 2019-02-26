#include "../Base/String.h"
#include "BoundingBox.h"
#include "Frustum.h"
#include "Polyhedron.h"

#include <cstdlib>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

void BoundingBox::Define(const Vector3<float>* vertices, size_t count)
{
	Undefine();
	Merge(vertices, count);
}

void BoundingBox::Define(const Frustum& frustum)
{
	Define(frustum._vertices, NUM_FRUSTUM_VERTICES);
}

void BoundingBox::Define(const Polyhedron& poly)
{
	Undefine();
	Merge(poly);
}

void BoundingBox::Define(const Sphere& sphere)
{
	const Vector3<float>& center = sphere._center;
	float radius = sphere._radius;

	_min = center + Vector3<float>(-radius, -radius, -radius);
	_max = center + Vector3<float>(radius, radius, radius);
}

void BoundingBox::Merge(const Vector3<float>* vertices, size_t count)
{
	while (count--)
		Merge(*vertices++);
}

void BoundingBox::Merge(const Frustum& frustum)
{
	Merge(frustum._vertices, NUM_FRUSTUM_VERTICES);
}

void BoundingBox::Merge(const Polyhedron& poly)
{
	for (size_t i = 0; i < poly._faces.Size(); ++i)
	{
		const Vector<Vector3<float>>& face = poly._faces[i];
		if (!face.IsEmpty())
			Merge(&face[0], face.Size());
	}
}

void BoundingBox::Merge(const Sphere& sphere)
{
	const Vector3<float>& center = sphere._center;
	float radius = sphere._radius;

	Merge(center + Vector3<float>(radius, radius, radius));
	Merge(center + Vector3<float>(-radius, -radius, -radius));
}

void BoundingBox::Clip(const BoundingBox& box)
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

void BoundingBox::Transform(const Matrix3x3<float>& transform)
{
	*this = Transformed(Matrix3x4<float>(transform));
}

void BoundingBox::Transform(const Matrix3x4<float>& transform)
{
	*this = Transformed(transform);
}

bool BoundingBox::FromString(const String& str)
{
	return FromString(str.CString());
}

bool BoundingBox::FromString(const char* str)
{
	size_t elements = String::CountElements(str, ' ');
	if (elements < 6)
		return false;

	char* ptr = (char*)str;
	_min._x = (float)strtod(ptr, &ptr);
	_min._y = (float)strtod(ptr, &ptr);
	_min._z = (float)strtod(ptr, &ptr);
	_max._x = (float)strtod(ptr, &ptr);
	_max._y = (float)strtod(ptr, &ptr);
	_max._z = (float)strtod(ptr, &ptr);

	return true;
}

BoundingBox BoundingBox::Transformed(const Matrix3x3<float>& transform) const
{
	return Transformed(Matrix3x4<float>(transform));
}

BoundingBox BoundingBox::Transformed(const Matrix3x4<float>& transform) const
{
	Vector3<float> oldCenter = Center();
	Vector3<float> oldEdge = _max - oldCenter;
	Vector3<float> newCenter = transform * oldCenter;
	Vector3<float> newEdge(
		Abs(transform._m00) * oldEdge._x + Abs(transform._m01) * oldEdge._y + Abs(transform._m02) * oldEdge._z,
		Abs(transform._m10) * oldEdge._x + Abs(transform._m11) * oldEdge._y + Abs(transform._m12) * oldEdge._z,
		Abs(transform._m20) * oldEdge._x + Abs(transform._m21) * oldEdge._y + Abs(transform._m22) * oldEdge._z
	);

	return BoundingBox(newCenter - newEdge, newCenter + newEdge);
}

Rect<float> BoundingBox::Projected(const Matrix4x4<float>& projection) const
{
	Vector3<float> projMin = _min;
	Vector3<float> projMax = _max;
	if (projMin._z < M_EPSILON)
		projMin._z = M_EPSILON;
	if (projMax._z < M_EPSILON)
		projMax._z = M_EPSILON;

	Vector3<float> vertices[8];
	vertices[0] = projMin;
	vertices[1] = Vector3<float>(projMax._x, projMin._y, projMin._z);
	vertices[2] = Vector3<float>(projMin._x, projMax._y, projMin._z);
	vertices[3] = Vector3<float>(projMax._x, projMax._y, projMin._z);
	vertices[4] = Vector3<float>(projMin._x, projMin._y, projMax._z);
	vertices[5] = Vector3<float>(projMax._x, projMin._y, projMax._z);
	vertices[6] = Vector3<float>(projMin._x, projMax._y, projMax._z);
	vertices[7] = projMax;

	Rect<float> rect;
	for (size_t i = 0; i < 8; ++i)
	{
		Vector3<float> projected = projection * vertices[i];
		rect.Merge(Vector2<float>(projected._x, projected._y));
	}

	return rect;
}

Intersection BoundingBox::IsInside(const Sphere& sphere) const
{
	float distSquared = 0;
	float temp;
	const Vector3<float>& center = sphere._center;

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

	float radius = sphere._radius;
	if (distSquared >= radius * radius)
		return OUTSIDE;
	else if (center._x - radius < _min._x || center._x + radius > _max._x || center._y - radius < _min._y ||
		center._y + radius > _max._y || center._z - radius < _min._z || center._z + radius > _max._z)
		return INTERSECTS;
	else
		return INSIDE;
}

Intersection BoundingBox::IsInsideFast(const Sphere& sphere) const
{
	float distSquared = 0;
	float temp;
	const Vector3<float>& center = sphere._center;

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

	float radius = sphere._radius;
	if (distSquared >= radius * radius)
		return OUTSIDE;
	else
		return INSIDE;
}

String BoundingBox::ToString() const
{
	return _min.ToString() + " " + _max.ToString();
}

}
