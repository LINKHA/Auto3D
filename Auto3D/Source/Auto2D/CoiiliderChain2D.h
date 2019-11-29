#pragma once
#include "Collider2D.h"

namespace Auto3D {

class AUTO_API CoiiliderChain2D : public Collider2D
{
	REGISTER_OBJECT_CLASS(CoiiliderChain2D, Collider2D)
public:
	/// Construct
	CoiiliderChain2D();
	/// Destructor
	virtual ~CoiiliderChain2D();
	/// Register object factory.
	static void RegisterObject();

	/// Set loop.
	void SetLoop(bool loop);
	/// Set vertex count.
	void SetVertexCount(unsigned count);
	/// Set vertex.
	void SetVertex(unsigned index, const Vector2F& vertex);
	/// Set vertices.
	void SetVertices(const TVector<Vector2F>& vertices);
	/// Set vertices attribute.
	void SetVerticesAttr(const TVector<unsigned char>& value);
	/// Return loop.
	bool GetLoop() const { return _loop; }
	/// Return vertex count.
	unsigned GetVertexCount() const { return _vertices.Size(); }
	/// Return vertex.
	const Vector2F& GetVertex(unsigned index) const { return (index < _vertices.Size()) ? _vertices[index] : Vector2F::ZERO; }
	/// Return vertices.
	const TVector<Vector2F>& GetVertices() const { return _vertices; }

	/// Return vertices attribute.
	TVector<unsigned char> GetVerticesAttr() const;

private:
	/// Recreate fixture.
	void RecreateFixture();

	/// Chain shape.
	b2ChainShape _chainShape;
	/// Loop.
	bool _loop;
	/// Vertices.
	TVector<Vector2F> _vertices;
};

}

