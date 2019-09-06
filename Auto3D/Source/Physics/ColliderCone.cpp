#include "ColliderCone.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D
{

ColliderCone::ColliderCone() :
	_radius(1.0f),
	_height(1.0f)
{
	_shapeType = ShapeType::CONE;

}

ColliderCone::~ColliderCone()
{
}

void ColliderCone::RegisterObject()
{
	RegisterFactory<ColliderCone>();
}

void ColliderCone::SetSize(float radius, float height)
{
	_radius = radius;
	_height = height;
	Resize(_radius, _height);
}

void ColliderCone::SetRadius(float radius)
{
	_radius = radius;
	Resize(_radius, _height);
}

void ColliderCone::SetHeight(float height)
{
	_height = height;
	Resize(_radius, _height);
}

void ColliderCone::Resize(float radius, float height)
{
	ReleaseShape();

	_shape.Reset();
	_shape = new btConeShape(radius, height);

	NotifyRigidBody();
}

}