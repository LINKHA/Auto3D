#include "ColliderCone.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D
{

AColliderCone::AColliderCone() :
	_radius(1.0f),
	_height(1.0f)
{
	_shapeType = EShapeType::CONE;

}

AColliderCone::~AColliderCone()
{
}

void AColliderCone::SetSize(float radius, float height)
{
	_radius = radius;
	_height = height;
	Resize(_radius, _height);
}

void AColliderCone::SetRadius(float radius)
{
	_radius = radius;
	Resize(_radius, _height);
}

void AColliderCone::SetHeight(float height)
{
	_height = height;
	Resize(_radius, _height);
}

void AColliderCone::Resize(float radius, float height)
{
	ReleaseShape();

	delete _shape;
	_shape = nullptr;
	_shape = new btConeShape(radius, height);

	NotifyRigidBody();
}

}