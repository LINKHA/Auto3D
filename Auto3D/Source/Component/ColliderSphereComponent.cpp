#include "Component/ColliderSphereComponent.h"
#include "Physics/PhysicsUtils.h"
#include "Physics/PhysicsWorld.h"


namespace Auto3D
{

AColliderSphereComponent::AColliderSphereComponent() :
	_size(1.0f)
{
	_shapeType = EShapeType::SPHERE;
}

AColliderSphereComponent::~AColliderSphereComponent()
{
}

void AColliderSphereComponent::SetSize(float scale)
{
	_size = scale;
	Resize(_size);
}


void AColliderSphereComponent::Resize(float radius)
{
	ReleaseShape();
	delete _shape;
	_shape = nullptr;
	_shape = new btSphereShape(radius);

	NotifyRigidBody();
}

}