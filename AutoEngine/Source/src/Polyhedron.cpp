// For conditions of distribution and use, see copyright notice in License.txt

#include "Frustum.h"
#include "Polyhedron.h"

#include "NewDef.h"

namespace Auto3D
{

Polyhedron::Polyhedron()
{
}

Polyhedron::Polyhedron(const Polyhedron& polyhedron) :
	faces(polyhedron.faces)
{
}

Polyhedron::Polyhedron(const VECTOR<VECTOR<Vector3F> >& faces) :
	faces(faces)
{
}

Polyhedron::Polyhedron(const BoundingBox& box)
{
	Define(box);
}

Polyhedron::Polyhedron(const Frustum& frustum)
{
	Define(frustum);
}

Polyhedron::~Polyhedron()
{
}

void Polyhedron::Define(const BoundingBox& box)
{
	Vector3F vertices[8];
	vertices[0] = box.min;
	vertices[1] = Vector3F(box.max._x, box.min._y, box.min._z);
	vertices[2] = Vector3F(box.min._x, box.max._y, box.min._z);
	vertices[3] = Vector3F(box.max._x, box.max._y, box.min._z);
	vertices[4] = Vector3F(box.min._x, box.min._y, box.max._z);
	vertices[5] = Vector3F(box.max._x, box.min._y, box.max._z);
	vertices[6] = Vector3F(box.min._x, box.max._y, box.max._z);
	vertices[7] = box.max;

	faces.resize(6);
	SetFace(0, vertices[3], vertices[7], vertices[5], vertices[1]);
	SetFace(1, vertices[6], vertices[2], vertices[0], vertices[4]);
	SetFace(2, vertices[6], vertices[7], vertices[3], vertices[2]);
	SetFace(3, vertices[1], vertices[5], vertices[4], vertices[0]);
	SetFace(4, vertices[7], vertices[6], vertices[4], vertices[5]);
	SetFace(5, vertices[2], vertices[3], vertices[1], vertices[0]);
}

void Polyhedron::Define(const Frustum& frustum)
{
	const Vector3F* vertices = frustum.vertices;

	faces.resize(6);
	SetFace(0, vertices[0], vertices[4], vertices[5], vertices[1]);
	SetFace(1, vertices[7], vertices[3], vertices[2], vertices[6]);
	SetFace(2, vertices[7], vertices[4], vertices[0], vertices[3]);
	SetFace(3, vertices[1], vertices[5], vertices[6], vertices[2]);
	SetFace(4, vertices[4], vertices[7], vertices[6], vertices[5]);
	SetFace(5, vertices[3], vertices[0], vertices[1], vertices[2]);
}

void Polyhedron::AddFace(const Vector3F& v0, const Vector3F& v1, const Vector3F& v2)
{
	faces.resize(faces.size() + 1);
	VECTOR<Vector3F>& face = faces[faces.size() - 1];
	face.resize(3);
	face[0] = v0;
	face[1] = v1;
	face[2] = v2;
}

void Polyhedron::AddFace(const Vector3F& v0, const Vector3F& v1, const Vector3F& v2, const Vector3F& v3)
{
	faces.resize(faces.size() + 1);
	VECTOR<Vector3F>& face = faces[faces.size() - 1];
	face.resize(4);
	face[0] = v0;
	face[1] = v1;
	face[2] = v2;
	face[3] = v3;
}

void Polyhedron::AddFace(const VECTOR<Vector3F>& face)
{
	faces.push_back(face);
}

void Polyhedron::Clip(const Plane& plane, VECTOR<Vector3F>& clippedVertices, VECTOR<Vector3F>& outFace)
{
	clippedVertices.clear();

	for (size_t i = 0; i < faces.size(); ++i)
	{
		VECTOR<Vector3F>& face = faces[i];
		Vector3F lastVertex;
		float lastDistance = 0.0f;

		outFace.clear();

		for (size_t j = 0; j < face.size(); ++j)
		{
			float distance = plane.Distance(face[j]);
			if (distance >= 0.0f)
			{
				if (lastDistance < 0.0f)
				{
					float t = lastDistance / (lastDistance - distance);
					Vector3F clippedVertex = lastVertex + t * (face[j] - lastVertex);
					outFace.push_back(clippedVertex);
					clippedVertices.push_back(clippedVertex);
				}

				outFace.push_back(face[j]);
			}
			else
			{
				if (lastDistance >= 0.0f && j != 0)
				{
					float t = lastDistance / (lastDistance - distance);
					Vector3F clippedVertex = lastVertex + t * (face[j] - lastVertex);
					outFace.push_back(clippedVertex);
					clippedVertices.push_back(clippedVertex);
				}
			}

			lastVertex = face[j];
			lastDistance = distance;
		}

		// Recheck the distances of the last and first vertices and add the final clipped vertex if applicable
		float distance = plane.Distance(face[0]);
		if ((lastDistance < 0.0f && distance >= 0.0f) || (lastDistance >= 0.0f && distance < 0.0f))
		{
			float t = lastDistance / (lastDistance - distance);
			Vector3F clippedVertex = lastVertex + t * (face[0] - lastVertex);
			outFace.push_back(clippedVertex);
			clippedVertices.push_back(clippedVertex);
		}

		// Do not keep faces which are less than triangles
		if (outFace.size() < 3)
			outFace.clear();

		face = outFace;
	}

	// Remove empty faces
	for (size_t i = faces.size() - 1; i < faces.size(); --i)
	{
		if (faces[i].empty())
			faces.erase(faces.begin()+i);
	}

	// Create a new face from the clipped vertices. First remove duplicates
	for (size_t i = 0; i < clippedVertices.size(); ++i)
	{
		for (size_t j = clippedVertices.size() - 1; j > i; --j)
		{
			if (clippedVertices[j].Equals(clippedVertices[i]))
				clippedVertices.erase(clippedVertices.begin()+j);
		}
	}

	if (clippedVertices.size() > 3)
	{
		outFace.clear();

		// Start with the first vertex
		outFace.push_back(clippedVertices.front());
		clippedVertices.erase(clippedVertices.begin());

		while (!clippedVertices.empty())
		{
			// Then add the vertex which is closest to the last added
			const Vector3F& lastAdded = outFace.back();
			float bestDistance = M_INFINITY;
			size_t bestIndex = 0;

			for (size_t i = 0; i < clippedVertices.size(); ++i)
			{
				float distance = (clippedVertices[i] - lastAdded).LengthSquared();
				if (distance < bestDistance)
				{
					bestDistance = distance;
					bestIndex = i;
				}
			}

			outFace.push_back(clippedVertices[bestIndex]);
			clippedVertices.erase(clippedVertices.begin() + bestIndex);
		}

		faces.push_back(outFace);
	}
}

void Polyhedron::Clip(const Plane& plane)
{
	VECTOR<Vector3F> clippedVertices;
	VECTOR<Vector3F> outFace;

	Clip(plane, clippedVertices, outFace);
}

void Polyhedron::Clip(const Frustum& frustum)
{
	VECTOR<Vector3F> clippedVertices;
	VECTOR<Vector3F> outFace;

	for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
		Clip(frustum.planes[i], clippedVertices, outFace);
}

void Polyhedron::Clip(const BoundingBox& box)
{
	VECTOR<Vector3F> clippedVertices;
	VECTOR<Vector3F> outFace;

	Vector3F vertices[8];
	vertices[0] = box.min;
	vertices[1] = Vector3F(box.max._x, box.min._y, box.min._z);
	vertices[2] = Vector3F(box.min._x, box.max._y, box.min._z);
	vertices[3] = Vector3F(box.max._x, box.max._y, box.min._z);
	vertices[4] = Vector3F(box.min._x, box.min._y, box.max._z);
	vertices[5] = Vector3F(box.max._x, box.min._y, box.max._z);
	vertices[6] = Vector3F(box.min._x, box.max._y, box.max._z);
	vertices[7] = box.max;

	Clip(Plane(vertices[5], vertices[7], vertices[3]), clippedVertices, outFace);
	Clip(Plane(vertices[0], vertices[2], vertices[6]), clippedVertices, outFace);
	Clip(Plane(vertices[3], vertices[7], vertices[6]), clippedVertices, outFace);
	Clip(Plane(vertices[4], vertices[5], vertices[1]), clippedVertices, outFace);
	Clip(Plane(vertices[4], vertices[6], vertices[7]), clippedVertices, outFace);
	Clip(Plane(vertices[1], vertices[3], vertices[2]), clippedVertices, outFace);
}

void Polyhedron::Clear()
{
	faces.clear();
}

void Polyhedron::Transform(const Matrix3x3F& transform)
{
	for (size_t i = 0; i < faces.size(); ++i)
	{
		VECTOR<Vector3F>& face = faces[i];
		for (size_t j = 0; j < face.size(); ++j)
			face[j] = transform * face[j];
	}
}

void Polyhedron::Transform(const Matrix3x4F& transform)
{
	for (size_t i = 0; i < faces.size(); ++i)
	{
		VECTOR<Vector3F>& face = faces[i];
		for (size_t j = 0; j < face.size(); ++j)
			face[j] = transform * face[j];
	}
}

Polyhedron Polyhedron::Transformed(const Matrix3x3F& transform) const
{
	Polyhedron ret;
	ret.faces.resize(faces.size());

	for (size_t i = 0; i < faces.size(); ++i)
	{
		const VECTOR<Vector3F>& face = faces[i];
		VECTOR<Vector3F>& newFace = ret.faces[i];
		newFace.resize(face.size());

		for (size_t j = 0; j < face.size(); ++j)
			newFace[j] = transform * face[j];
	}

	return ret;
}

Polyhedron Polyhedron::Transformed(const Matrix3x4F& transform) const
{
	Polyhedron ret;
	ret.faces.resize(faces.size());

	for (size_t i = 0; i < faces.size(); ++i)
	{
		const VECTOR<Vector3F>& face = faces[i];
		VECTOR<Vector3F>& newFace = ret.faces[i];
		newFace.resize(face.size());

		for (size_t j = 0; j < face.size(); ++j)
			newFace[j] = transform * face[j];
	}

	return ret;
}

void Polyhedron::SetFace(size_t index, const Vector3F& v0, const Vector3F& v1, const Vector3F& v2)
{
	VECTOR<Vector3F>& face = faces[index];
	face.resize(3);
	face[0] = v0;
	face[1] = v1;
	face[2] = v2;
}

void Polyhedron::SetFace(size_t index, const Vector3F& v0, const Vector3F& v1, const Vector3F& v2, const Vector3F& v3)
{
	VECTOR<Vector3F>& face = faces[index];
	face.resize(4);
	face[0] = v0;
	face[1] = v1;
	face[2] = v2;
	face[3] = v3;
}

}
