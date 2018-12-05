#include "ColliderBox2D.h"
#include "Ambient.h"
#include "NewDef.h"

namespace Auto3D {

static const Vector2 DEFAULT_BOX_SIZE(0.01f, 0.01f);

ColliderBox2D::ColliderBox2D(Ambient* ambient)
	: Super(ambient)
	, _size(DEFAULT_BOX_SIZE)
	, _center(Vector2::zero)
	, _angle(0.0f)
{
	float halfWidth = _size.x * 0.5f * _cachedWorldScale.x;
	float halfHeight = _size.y * 0.5f * _cachedWorldScale.y;
	_boxShape.SetAsBox(halfWidth, halfHeight);
	_fixtureDef.shape = &_boxShape;
}


ColliderBox2D::~ColliderBox2D()
{
}

void ColliderBox2D::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<ColliderBox2D>(SCENE_ATTACH);
}

void ColliderBox2D::Start()
{
	Super::Start();

}

void ColliderBox2D::SetSize(const Vector2& size)
{
	if (size == _size)
		return;

	_size = size;

	recreateFixture();
}

void ColliderBox2D::SetSize(float width, float height)
{
	SetSize(Vector2(width, height));
}

void ColliderBox2D::SetCenter(const Vector2& center)
{
	if (center == _center)
		return;

	_center = center;

	recreateFixture();
}

void ColliderBox2D::SetCenter(float x, float y)
{
	SetCenter(Vector2(x, y));
}

void ColliderBox2D::SetAngle(float angle)
{
	if (angle == _angle)
		return;

	_angle = angle;

	recreateFixture();
}

void ColliderBox2D::recreateFixture()
{
	ReleaseFixture();

	float worldScaleX = _cachedWorldScale.x;
	float worldScaleY = _cachedWorldScale.y;
	float halfWidth = _size.x * 0.5f * worldScaleX;
	float halfHeight = _size.y * 0.5f * worldScaleY;
	Vector2 scaledCenter = _center * Vector2(worldScaleX, worldScaleY);

	if (scaledCenter == Vector2::zero && _angle == 0.0f)
		_boxShape.SetAsBox(halfWidth, halfHeight);
	else
		_boxShape.SetAsBox(halfWidth, halfHeight, ToB2Vec2(scaledCenter), _angle * MATH_DEGTORAD);

	CreateFixture();
}

}