// For conditions of distribution and use, see copyright notice in License.txt

#include <cstdlib>
#include "BoundingBox.h"
#include "Frustum.h"
#include "Polyhedron.h"

#include "NewDef.h"

namespace Auto3D
{

void BoundingBox::Define(const Vector3F* vertices, size_t count)
{
	Undefine();
	Merge(vertices, count);
}

void BoundingBox::Define(const Frustum& frustum)
{
	Define(frustum.vertices, NUM_FRUSTUM_VERTICES);
}

void BoundingBox::Define(const Polyhedron& poly)
{
	Undefine();
	Merge(poly);
}

void BoundingBox::Define(const Sphere& sphere)
{
	const Vector3F& center = sphere.center;
	float radius = sphere.radius;

	min = center + Vector3F(-radius, -radius, -radius);
	max = center + Vector3F(radius, radius, radius);
}

void BoundingBox::Merge(const Vector3F* vertices, size_t count)
{
	while (count--)
		Merge(*vertices++);
}

void BoundingBox::Merge(const Frustum& frustum)
{
	Merge(frustum.vertices, NUM_FRUSTUM_VERTICES);
}

void BoundingBox::Merge(const Polyhedron& poly)
{
	for (size_t i = 0; i < poly.faces.size(); ++i)
	{
		const VECTOR<Vector3F>& face = poly.faces[i];
		if (!face.empty())
			Merge(&face[0], face.size());
	}
}

void BoundingBox::Merge(const Sphere& sphere)
{
	const Vector3F& center = sphere.center;
	float radius = sphere.radius;

	Merge(center + Vector3F(radius, radius, radius));
	Merge(center + Vector3F(-radius, -radius, -radius));
}

void BoundingBox::Clip(const BoundingBox& box)
{
	if (box.min._x > min._x)
		min._x = box.min._x;
	if (box.max._x < max._x)
		max._x = box.max._x;
	if (box.min._y > min._y)
		min._y = box.min._y;
	if (box.max._y < max._y)
		max._y = box.max._y;
	if (box.min._z > min._z)
		min._z = box.min._z;
	if (box.max._z < max._z)
		max._z = box.max._z;

	if (min._x > max._x)
		Swap(min._x, max._x);
	if (min._y > max._y)
		Swap(min._y, max._y);
	if (min._z > max._z)
		Swap(min._z, max._z);
}

void BoundingBox::Transform(const Matrix3x3F& transform)
{
	*this = Transformed(Matrix3x4F(transform));
}

void BoundingBox::Transform(const Matrix3x4F& transform)
{
	*this = Transformed(transform);
}

bool BoundingBox::FromString(const STRING& str)
{
	return FromString(str.CStr());
}

bool BoundingBox::FromString(const char* str)
{
	size_t elements = STRING::CountElements(str, ' ');
	if (elements < 6)
		return false;

	char* ptr = (char*)str;
	min._x = (float)strtod(ptr, &ptr);
	min._y = (float)strtod(ptr, &ptr);
	min._z = (float)strtod(ptr, &ptr);
	max._x = (float)strtod(ptr, &ptr);
	max._y = (float)strtod(ptr, &ptr);
	max._z = (float)strtod(ptr, &ptr);

	return true;
}

BoundingBox BoundingBox::Transformed(const Matrix3x3F& transform) const
{
	return Transformed(Matrix3x4F(transform));
}

BoundingBox BoundingBox::Transformed(const Matrix3x4F& transform) const
{
	Vector3F oldCenter = Center();
	Vector3F oldEdge = max - oldCenter;
	Vector3F newCenter = transform * oldCenter;
	Vector3F newEdge(
		Abs(transform._m00) * oldEdge._x + Abs(transform._m01) * oldEdge._y + Abs(transform._m02) * oldEdge._z,
		Abs(transform._m10) * oldEdge._x + Abs(transform._m11) * oldEdge._y + Abs(transform._m12) * oldEdge._z,
		Abs(transform._m20) * oldEdge._x + Abs(transform._m21) * oldEdge._y + Abs(transform._m22) * oldEdge._z
	);

	return BoundingBox(newCenter - newEdge, newCenter + newEdge);
}

Rect BoundingBox::Projected(const Matrix4x4F& projection) const
{
	Vector3F projMin = min;
	Vector3F projMax = max;
	if (projMin._z < M_EPSILON)
		projMin._z = M_EPSILON;
	if (projMax._z < M_EPSILON)
		projMax._z = M_EPSILON;

	Vector3F vertices[8];
	vertices[0] = projMin;
	vertices[1] = Vector3F(projMax._x, projMin._y, projMin._z);
	vertices[2] = Vector3F(projMin._x, projMax._y, projMin._z);
	vertices[3] = Vector3F(projMax._x, projMax._y, projMin._z);
	vertices[4] = Vector3F(projMin._x, projMin._y, projMax._z);
	vertices[5] = Vector3F(projMax._x, projMin._y, projMax._z);
	vertices[6] = Vector3F(projMin._x, projMax._y, projMax._z);
	vertices[7] = projMax;

	Rect rect;
	for (size_t i = 0; i < 8; ++i)
	{
		Vector3F projected = projection * vertices[i];
		rect.Merge(Vector2F(projected._x, projected._y));
	}

	return rect;
}

Intersection BoundingBox::IsInside(const Sphere& sphere) const
{
	float distSquared = 0;
	float temp;
	const Vector3F& center = sphere.center;

	if (center._x < min._x)
	{
		temp = center._x - min._x;
		distSquared += temp * temp;
	}
	else if (center._x > max._x)
	{
		temp = center._x - max._x;
		distSquared += temp * temp;
	}
	if (center._y < min._y)
	{
		temp = center._y - min._y;
		distSquared += temp * temp;
	}
	else if (center._y > max._y)
	{
		temp = center._y - max._y;
		distSquared += temp * temp;
	}
	if (center._z < min._z)
	{
		temp = center._z - min._z;
		distSquared += temp * temp;
	}
	else if (center._z > max._z)
	{
		temp = center._z - max._z;
		distSquared += temp * temp;
	}

	float radius = sphere.radius;
	if (distSquared >= radius * radius)
		return OUTSIDE;
	else if (center._x - radius < min._x || center._x + radius > max._x || center._y - radius < min._y ||
		center._y + radius > max._y || center._z - radius < min._z || center._z + radius > max._z)
		return INTERSECTS;
	else
		return INSIDE;
}

Intersection BoundingBox::IsInsideFast(const Sphere& sphere) const
{
	float distSquared = 0;
	float temp;
	const Vector3F& center = sphere.center;

	if (center._x < min._x)
	{
		temp = center._x - min._x;
		distSquared += temp * temp;
	}
	else if (center._x > max._x)
	{
		temp = center._x - max._x;
		distSquared += temp * temp;
	}
	if (center._y < min._y)
	{
		temp = center._y - min._y;
		distSquared += temp * temp;
	}
	else if (center._y > max._y)
	{
		temp = center._y - max._y;
		distSquared += temp * temp;
	}
	if (center._z < min._z)
	{
		temp = center._z - min._z;
		distSquared += temp * temp;
	}
	else if (center._z > max._z)
	{
		temp = center._z - max._z;
		distSquared += temp * temp;
	}

	float radius = sphere.radius;
	if (distSquared >= radius * radius)
		return OUTSIDE;
	else
		return INSIDE;
}

STRING BoundingBox::ToString() const
{
	return min.ToString() + " " + max.ToString();
}

}
