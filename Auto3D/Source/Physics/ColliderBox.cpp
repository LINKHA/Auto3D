#include "ColliderBox.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D 
{


ColliderBox::ColliderBox(): 
	_size(Vector3F::ONE)
{
	_shapeType = ShapeType::BOX;
}

ColliderBox::~ColliderBox()
{
}

void ColliderBox::RegisterObject()
{
	RegisterFactory<ColliderBox>();
}

void ColliderBox::SetSize(const Vector3F& vec)
{
	_size = vec;
	Resize(_size);
}

void ColliderBox::SetSize(float x, float y, float z)
{
	_size = Vector3F(x, y, z);
	Resize(_size);
}

void ColliderBox::SetSize(float scale)
{
	_size = Vector3F(scale, scale, scale);
	Resize(_size);
}


void ColliderBox::Resize(const Vector3F& vec)
{
	ReleaseShape();
	_shape.Reset();
	_shape = new btBoxShape(ToBtVector3(vec));
	
	NotifyRigidBody();
}

}