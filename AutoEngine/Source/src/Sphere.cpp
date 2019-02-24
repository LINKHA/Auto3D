// For conditions of distribution and use, see copyright notice in License.txt
#include "BoundingBox.h"
#include "Frustum.h"
#include "Polyhedron.h"
#include "Sphere.h"

#include "NewDef.h"

namespace Auto3D
{

void Sphere::Define(const Vector3F* vertices, size_t count)
{
	Undefine();
	Merge(vertices, count);
}

void Sphere::Define(const BoundingBox& box)
{
	const Vector3F& min = box.min;
	const Vector3F& max = box.max;

	Undefine();
	Merge(min);
	Merge(Vector3F(max._x, min._y, min._z));
	Merge(Vector3F(min._x, max._y, min._z));
	Merge(Vector3F(max._x, max._y, min._z));
	Merge(Vector3F(min._x, min._y, max._z));
	Merge(Vector3F(max._x, min._y, max._z));
	Merge(Vector3F(min._x, max._y, max._z));
	Merge(max);
}

void Sphere::Define(const Frustum& frustum)
{
	Define(frustum.vertices, NUM_FRUSTUM_VERTICES);
}

void Sphere::Define(const Polyhedron& poly)
{
	Undefine();
	Merge(poly);
}

void Sphere::Merge(const Vector3F* vertices, size_t count)
{
	while (count--)
		Merge(*vertices++);
}

void Sphere::Merge(const BoundingBox& box)
{
	const Vector3F& min = box.min;
	const Vector3F& max = box.max;

	Merge(min);
	Merge(Vector3F(max._x, min._y, min._z));
	Merge(Vector3F(min._x, max._y, min._z));
	Merge(Vector3F(max._x, max._y, min._z));
	Merge(Vector3F(min._x, min._y, max._z));
	Merge(Vector3F(max._x, min._y, max._z));
	Merge(Vector3F(min._x, max._y, max._z));
	Merge(max);
}

void Sphere::Merge(const Frustum& frustum)
{
	const Vector3F* vertices = frustum.vertices;
	Merge(vertices, NUM_FRUSTUM_VERTICES);
}

void Sphere::Merge(const Polyhedron& poly)
{
	for (size_t i = 0; i < poly.faces.size(); ++i)
	{
		const VECTOR<Vector3F>& face = poly.faces[i];
		if (!face.empty())
			Merge(&face[0], face.size());
	}
}

void Sphere::Merge(const Sphere& sphere)
{
	// If undefined, set initial dimensions
	if (!IsDefined())
	{
		center = sphere.center;
		radius = sphere.radius;
		return;
	}

	Vector3F offset = sphere.center - center;
	float dist = offset.Length();

	// If sphere fits inside, do nothing
	if (dist + sphere.radius < radius)
		return;

	// If we fit inside the other sphere, become it
	if (dist + radius < sphere.radius)
	{
		center = sphere.center;
		radius = sphere.radius;
	}
	else
	{
		Vector3F normalizedOffset = offset / dist;

		Vector3F min = center - radius * normalizedOffset;
		Vector3F max = sphere.center + sphere.radius * normalizedOffset;
		center = (min + max) * 0.5f;
		radius = (max - center).Length();
	}
}

Intersection Sphere::IsInside(const BoundingBox& box) const
{
	float radiusSquared = radius * radius;
	float distSquared = 0;
	float temp;
	Vector3F min = box.min;
	Vector3F max = box.max;

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

	if (distSquared >= radiusSquared)
		return OUTSIDE;

	min -= center;
	max -= center;

	Vector3F tempVec = min; // - - -
	if (tempVec.LengthSquared() >= radiusSquared)
		return INTERSECTS;
	tempVec._x = max._x; // + - -
	if (tempVec.LengthSquared() >= radiusSquared)
		return INTERSECTS;
	tempVec._y = max._y; // + + -
	if (tempVec.LengthSquared() >= radiusSquared)
		return INTERSECTS;
	tempVec._x = min._x; // - + -
	if (tempVec.LengthSquared() >= radiusSquared)
		return INTERSECTS;
	tempVec._z = max._z; // - + +
	if (tempVec.LengthSquared() >= radiusSquared)
		return INTERSECTS;
	tempVec._y = min._y; // - - +
	if (tempVec.LengthSquared() >= radiusSquared)
		return INTERSECTS;
	tempVec._x = max._x; // + - +
	if (tempVec.LengthSquared() >= radiusSquared)
		return INTERSECTS;
	tempVec._y = max._y; // + + +
	if (tempVec.LengthSquared() >= radiusSquared)
		return INTERSECTS;

	return INSIDE;
}

Intersection Sphere::IsInsideFast(const BoundingBox& box) const
{
	float radiusSquared = radius * radius;
	float distSquared = 0;
	float temp;
	Vector3F min = box.min;
	Vector3F max = box.max;

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

	if (distSquared >= radiusSquared)
		return OUTSIDE;
	else
		return INSIDE;
}

}
