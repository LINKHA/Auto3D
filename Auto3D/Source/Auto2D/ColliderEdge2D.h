#pragma once
#include "Collider2D.h"

namespace Auto3D {

class AUTO_API ColliderEdge2D : public Collider2D
{
	REGISTER_OBJECT_CLASS(ColliderEdge2D, Collider2D)
public:
	/// Construct
	ColliderEdge2D();
	/// Destructor
	virtual ~ColliderEdge2D();
	/// Register object factory.
	static void RegisterObject();
	

	/// Set vertex 1.
	void SetVertex1(const Vector2F& vertex);
	/// Set vertex 2.
	void SetVertex2(const Vector2F& vertex);
	/// Set vertices.
	void SetVertices(const Vector2F& vertex1, const Vector2F& vertex2);

	/// Return vertex 1.
	const Vector2F& GetVertex1() const { return _vertex1; }

	/// Return vertex 2.
	const Vector2F& GetVertex2() const { return _vertex2; }

private:
	/// Recreate fixture.
	void RecreateFixture();

	/// Edge shape.
	b2EdgeShape _edgeShape;
	/// Vertex 1.
	Vector2F _vertex1;
	/// Vertex 2.
	Vector2F _vertex2;
};

}

