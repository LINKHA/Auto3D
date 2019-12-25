#pragma once
#include "Collider2D.h"

namespace Auto3D {

class AUTO_API ACoiiliderChain2D : public ACollider2D
{
	DECLARE_CLASS(ACoiiliderChain2D, ACollider2D)
public:
	/// Construct
	ACoiiliderChain2D();
	/// Destructor
	virtual ~ACoiiliderChain2D();

	/// Set loop.
	void SetLoop(bool loop);
	/// Set vertex count.
	void SetVertexCount(unsigned count);
	/// Set vertex.
	void SetVertex(unsigned index, const TVector2F& vertex);
	/// Set vertices.
	void SetVertices(const TVector<TVector2F>& vertices);
	/// Set vertices attribute.
	void SetVerticesAttr(const TVector<unsigned char>& value);
	/// Return loop.
	bool GetLoop() const { return _loop; }
	/// Return vertex count.
	unsigned GetVertexCount() const { return _vertices.Size(); }
	/// Return vertex.
	const TVector2F& GetVertex(unsigned index) const { return (index < _vertices.Size()) ? _vertices[index] : TVector2F::ZERO; }
	/// Return vertices.
	const TVector<TVector2F>& GetVertices() const { return _vertices; }

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
	TVector<TVector2F> _vertices;
};

}

