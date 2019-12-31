#include "ColliderBox2D.h"

namespace Auto3D {

static const TVector2F DEFAULT_BOX_SIZE(0.01f, 0.01f);

AColliderBox2D::AColliderBox2D() :
	_size(DEFAULT_BOX_SIZE)
{
	float halfWidth = _size._x * 0.5f;
	float halfHeight = _size._y * 0.5f;
	_boxShape.SetAsBox(halfWidth, halfHeight);
	_fixtureDef.shape = &_boxShape;
}

AColliderBox2D::~AColliderBox2D() = default;

void AColliderBox2D::SetSize(const TVector2F& size)
{
	_size = size;
	RecreateFixture();
}

void AColliderBox2D::SetSize(float x, float y)
{
	_size = TVector2F(x, y);
	RecreateFixture();
}

void AColliderBox2D::SetSize(float scale)
{
	_size = TVector2F(scale, scale);
	RecreateFixture();
}

void AColliderBox2D::RecreateFixture()
{
	ReleaseFixture();
	_boxShape.SetAsBox(_size._x, _size._y);
	CreateFixture();

}

}