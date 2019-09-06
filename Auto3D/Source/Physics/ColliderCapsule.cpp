#include "ColliderCapsule.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D
{

ColliderCapsule::ColliderCapsule() :
	_radius(1.0f),
	_height(1.0f)
{
	_shapeType = ShapeType::CAPSULE;
	
}

ColliderCapsule::~ColliderCapsule()
{
}

void ColliderCapsule::RegisterObject()
{
	RegisterFactory<ColliderCapsule>();
}

void ColliderCapsule::SetSize(float radius, float height)
{
	_radius = radius;
	_height = height;
	Resize(_radius, _height);
}

void ColliderCapsule::SetRadius(float radius)
{
	_radius = radius;
	Resize(_radius, _height);
}

void ColliderCapsule::SetHeight(float height)
{
	_height = height;
	Resize(_radius, _height);
}

void ColliderCapsule::Resize(float radius, float height)
{
	ReleaseShape();

	_shape.Reset();
	_shape = new btCapsuleShape(radius, height);

	NotifyRigidBody();
}

}