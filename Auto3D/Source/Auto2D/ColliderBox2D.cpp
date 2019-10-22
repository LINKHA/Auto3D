#include "ColliderBox2D.h"

namespace Auto3D {

static const Vector2F DEFAULT_BOX_SIZE(0.01f, 0.01f);

ColliderBox2D::ColliderBox2D() :
	_size(DEFAULT_BOX_SIZE)
{
	float halfWidth = _size._x * 0.5f;
	float halfHeight = _size._y * 0.5f;
	_boxShape.SetAsBox(halfWidth, halfHeight);
	_fixtureDef.shape = &_boxShape;
}

ColliderBox2D::~ColliderBox2D() = default;

void ColliderBox2D::RegisterObject()
{
	RegisterFactory<ColliderBox2D>();
}

void ColliderBox2D::SetSize(const Vector2F& size)
{
	_size = size;
	Resize(_size);
}

void ColliderBox2D::SetSize(float x, float y)
{
	_size = Vector2F(x, y);
	Resize(_size);
}

void ColliderBox2D::SetSize(float scale)
{
	_size = Vector2F(scale, scale);
	Resize(_size);
}

void ColliderBox2D::Resize(const Vector2F& vec)
{
	ReleaseShape();
	_boxShape.SetAsBox(vec._x, vec._y);
	NotifyRigidBody();

}

}