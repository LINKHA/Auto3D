#include "ColliderBox.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D 
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(AColliderBox)
	.constructor<>()
	.property("size", &AColliderBox::GetSize, static_cast<void(AColliderBox::*)(const TVector3F&)>(&AColliderBox::SetSize))
	(
		metadata(SERIALIZABLE, "")
	)
	;
}

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
	_shape.Reset();
	_shape = new btBoxShape(ToBtVector3(vec));
	
	NotifyRigidBody();
}

}