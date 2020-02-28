#include "Component/ColliderConeComponent.h"
#include "Physics/PhysicsUtils.h"
#include "Physics/PhysicsWorld.h"


namespace Auto3D
{

AColliderConeComponent::AColliderConeComponent() :
	_radius(1.0f),
	_height(1.0f)
{
	_shapeType = EShapeType::CONE;

}

AColliderConeComponent::~AColliderConeComponent()
{
}

void AColliderConeComponent::SetSize(float radius, float height)
{
	_radius = radius;
	_height = height;
	Resize(_radius, _height);
}

void AColliderConeComponent::SetRadius(float radius)
{
	_radius = radius;
	Resize(_radius, _height);
}

void AColliderConeComponent::SetHeight(float height)
{
	_height = height;
	Resize(_radius, _height);
}

void AColliderConeComponent::Resize(float radius, float height)
{
	ReleaseShape();

	delete _shape;
	_shape = nullptr;
	_shape = new btConeShape(radius, height);

	NotifyRigidBody();
}

}