#include "Component/ColliderCapsuleComponent.h"
#include "Physics/PhysicsUtils.h"
#include "Physics/PhysicsWorld.h"


namespace Auto3D
{

AColliderCapsuleComponent::AColliderCapsuleComponent() :
	_radius(1.0f),
	_height(1.0f)
{
	_shapeType = EShapeType::CAPSULE;
	
}

AColliderCapsuleComponent::~AColliderCapsuleComponent()
{
}

void AColliderCapsuleComponent::SetSize(float radius, float height)
{
	_radius = radius;
	_height = height;
	Resize(_radius, _height);
}

void AColliderCapsuleComponent::SetRadius(float radius)
{
	_radius = radius;
	Resize(_radius, _height);
}

void AColliderCapsuleComponent::SetHeight(float height)
{
	_height = height;
	Resize(_radius, _height);
}

void AColliderCapsuleComponent::Resize(float radius, float height)
{
	ReleaseShape();

	delete _shape;
	_shape = nullptr;
	_shape = new btCapsuleShape(radius, height);

	NotifyRigidBody();
}

}