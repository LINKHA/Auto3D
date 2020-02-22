#include "ColliderBox.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D 
{

AColliderBox::AColliderBox(): 
	_size(TVector3F::ONE)
{
	_shapeType = EShapeType::BOX;
}

AColliderBox::~AColliderBox()
{
}

void AColliderBox::SetSize(const TVector3F& vec)
{
	_size = vec;
	Resize(_size);
}

void AColliderBox::SetSize(float x, float y, float z)
{
	_size = TVector3F(x, y, z);
	Resize(_size);
}

void AColliderBox::SetSize(float scale)
{
	_size = TVector3F(scale, scale, scale);
	Resize(_size);
}


void AColliderBox::Resize(const TVector3F& vec)
{
	ReleaseShape();
	_shape = new btBoxShape(ToBtVector3(vec));
	
	NotifyRigidBody();
}

}