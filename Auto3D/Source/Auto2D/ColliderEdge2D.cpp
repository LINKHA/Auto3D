#include "ColliderEdge2D.h"
#include "Physics/PhysicsUtils.h"

namespace Auto3D {

static const TVector2F DEFAULT_VERTEX1(-0.01f, 0.0f);
static const TVector2F DEFAULT_VERTEX2(0.01f, 0.0f);

ColliderEdge2D::ColliderEdge2D() :
	_vertex1(DEFAULT_VERTEX1),
	_vertex2(DEFAULT_VERTEX2)
{
	_edgeShape.Set(ToB2Vector2(_vertex1), ToB2Vector2(_vertex2));

	_fixtureDef.shape = &_edgeShape;
}

ColliderEdge2D::~ColliderEdge2D() = default;

void ColliderEdge2D::RegisterObject()
{
	RegisterFactory<ColliderEdge2D>();
}

void ColliderEdge2D::SetVertex1(const TVector2F& vertex)
{
	SetVertices(vertex, _vertex2);
}

void ColliderEdge2D::SetVertex2(const TVector2F& vertex)
{
	SetVertices(_vertex1, vertex);
}

void ColliderEdge2D::SetVertices(const TVector2F& vertex1, const TVector2F& vertex2)
{
	if (vertex1 == _vertex1 && vertex2 == _vertex2)
		return;

	_vertex1 = vertex1;
	_vertex2 = vertex2;

	RecreateFixture();
}

void ColliderEdge2D::RecreateFixture()
{
	ReleaseFixture();

	_edgeShape.Set(ToB2Vector2(_vertex1), ToB2Vector2(_vertex2));

	CreateFixture();
}

}