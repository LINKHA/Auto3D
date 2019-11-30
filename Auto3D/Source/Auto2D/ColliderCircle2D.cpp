#include "ColliderCircle2D.h"
#include "Physics/PhysicsUtils.h"

namespace Auto3D {

static const float DEFAULT_CLRCLE_RADIUS(0.01f);

ColliderCircle2D::ColliderCircle2D() :
	_radius(DEFAULT_CLRCLE_RADIUS),
	_center(TVector2F::ZERO)
{
	_circleShape.m_radius = DEFAULT_CLRCLE_RADIUS;
	_fixtureDef.shape = &_circleShape;
}

ColliderCircle2D::~ColliderCircle2D() = default;

void ColliderCircle2D::RegisterObject()
{
	RegisterFactory<ColliderCircle2D>();
}

void ColliderCircle2D::SetRadius(float radius)
{
	if (radius == _radius)
		return;

	_radius = radius;

	RecreateFixture();
}


void ColliderCircle2D::SetCenter(const TVector2F& center)
{
	if (center == _center)
		return;

	_center = center;

	RecreateFixture();
}


void ColliderCircle2D::SetCenter(float x, float y)
{
	SetCenter(TVector2F(x, y));
}

void ColliderCircle2D::RecreateFixture()
{
	ReleaseFixture(); 

	_circleShape.m_p = ToB2Vector2(_center);
	_circleShape.m_radius = _radius;

	CreateFixture();
}

}