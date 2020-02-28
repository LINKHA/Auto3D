#include "Component/ColliderBoxComponent.h"
#include "Physics/PhysicsUtils.h"
#include "Physics/PhysicsWorld.h"


namespace Auto3D 
{

AColliderBoxComponent::AColliderBoxComponent(): 
	_size(TVector3F::ONE)
{
	_shapeType = EShapeType::BOX;
}

AColliderBoxComponent::~AColliderBoxComponent()
{
}

void AColliderBoxComponent::SetSize(const TVector3F& vec)
{
	_size = vec;
	Resize(_size);
}

void AColliderBoxComponent::SetSize(float x, float y, float z)
{
	_size = TVector3F(x, y, z);
	Resize(_size);
}

void AColliderBoxComponent::SetSize(float scale)
{
	_size = TVector3F(scale, scale, scale);
	Resize(_size);
}


void AColliderBoxComponent::Resize(const TVector3F& vec)
{
	ReleaseShape();
	_shape = new btBoxShape(ToBtVector3(vec));
	
	NotifyRigidBody();
}

}