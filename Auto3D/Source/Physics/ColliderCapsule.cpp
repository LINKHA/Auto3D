#include "ColliderCapsule.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(AColliderCapsule)
	.constructor<>()
	.property("radius", &AColliderCapsule::GetRadius, &AColliderCapsule::SetRadius)
	(
		metadata(SERIALIZABLE, "")
	)
	.property("height", &AColliderCapsule::GetHeight, &AColliderCapsule::SetHeight)
	(
		metadata(SERIALIZABLE, "")
	)
	;
}

AColliderCapsule::AColliderCapsule() :
	_radius(1.0f),
	_height(1.0f)
{
	_shapeType = EShapeType::CAPSULE;
	
}

AColliderCapsule::~AColliderCapsule()
{
}

void AColliderCapsule::SetSize(float radius, float height)
{
	_radius = radius;
	_height = height;
	Resize(_radius, _height);
}

void AColliderCapsule::SetRadius(float radius)
{
	_radius = radius;
	Resize(_radius, _height);
}

void AColliderCapsule::SetHeight(float height)
{
	_height = height;
	Resize(_radius, _height);
}

void AColliderCapsule::Resize(float radius, float height)
{
	ReleaseShape();

	_shape.Reset();
	_shape = new btCapsuleShape(radius, height);

	NotifyRigidBody();
}

}