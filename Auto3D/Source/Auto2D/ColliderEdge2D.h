#pragma once
#include "Collider2D.h"

namespace Auto3D {

class AUTO_API AColliderEdge2D : public ACollider2D
{
	REGISTER_OBJECT(AColliderEdge2D, ACollider2D)
public:
	/// Construct
	AColliderEdge2D();
	/// Destructor
	virtual ~AColliderEdge2D();
	/// Register object factory.
	static void RegisterObject();
	

	/// Set vertex 1.
	void SetVertex1(const TVector2F& vertex);
	/// Set vertex 2.
	void SetVertex2(const TVector2F& vertex);
	/// Set vertices.
	void SetVertices(const TVector2F& vertex1, const TVector2F& vertex2);

	/// Return vertex 1.
	const TVector2F& GetVertex1() const { return _vertex1; }

	/// Return vertex 2.
	const TVector2F& GetVertex2() const { return _vertex2; }

private:
	/// Recreate fixture.
	void RecreateFixture();

	/// Edge shape.
	b2EdgeShape _edgeShape;
	/// Vertex 1.
	TVector2F _vertex1;
	/// Vertex 2.
	TVector2F _vertex2;
};

}

