// For conditions of distribution and use, see copyright notice in License.txt

#pragma once

#include "Vector3.h"

namespace Auto3D
{
template<typename _Ty> class Matrix3x4;
template<typename _Ty> class Matrix3x3;
class BoundingBox;
class Frustum;
class Plane;

/// A convex volume built from polygon faces.
class Polyhedron
{
public:
	/// Polygon faces.
	VECTOR<VECTOR<Vector3F> > faces;

	/// Construct empty.
	Polyhedron();
	/// Copy-construct.
	Polyhedron(const Polyhedron& polyhedron);
	/// Construct from a list of faces.
	Polyhedron(const VECTOR<VECTOR<Vector3F> >& faces);
	/// Construct from a bounding box.
	Polyhedron(const BoundingBox& box);
	/// Construct from a frustum.
	Polyhedron(const Frustum& frustum);
	/// Destruct.
	~Polyhedron();

	/// Define from a bounding box.
	void Define(const BoundingBox& box);
	/// Define from a frustum.
	void Define(const Frustum& frustum);
	/// Add a triangle face.
	void AddFace(const Vector3F& v0, const Vector3F& v1, const Vector3F& v2);
	/// Add a quadrilateral face.
	void AddFace(const Vector3F& v0, const Vector3F& v1, const Vector3F& v2, const Vector3F& v3);
	/// Add an arbitrary face.
	void AddFace(const VECTOR<Vector3F>& face);
	/// Clip with a plane using supplied work vectors. When clipping with several planes in a succession these can be the same to avoid repeated dynamic memory allocation.
	void Clip(const Plane& plane, VECTOR<Vector3F>& outFace, VECTOR<Vector3F>& clippedVertices);
	/// Clip with a plane.
	void Clip(const Plane& plane);
	/// Clip with a bounding box.
	void Clip(const BoundingBox& box);
	/// Clip with a frustum.
	void Clip(const Frustum& box);
	/// Clear all faces.
	void Clear();
	/// Transform with a 3x3 matrix.
	void Transform(const Matrix3x3F& transform);
	/// Transform with a 3x4 matrix.
	void Transform(const Matrix3x4F& transform);

	/// Return transformed with a 3x3 matrix.
	Polyhedron Transformed(const Matrix3x3F& transform) const;
	/// Return transformed with a 3x4 matrix.
	Polyhedron Transformed(const Matrix3x4F& transform) const;
	/// Return whether has no faces.
	bool IsEmpty() const { return faces.empty(); }

private:
	/// Set a triangle face by index.
	void SetFace(size_t index, const Vector3F& v0, const Vector3F& v1, const Vector3F& v2);
	/// Set a quadrilateral face by index.
	void SetFace(size_t index, const Vector3F& v0, const Vector3F& v1, const Vector3F& v2, const Vector3F& v3);
};

}
