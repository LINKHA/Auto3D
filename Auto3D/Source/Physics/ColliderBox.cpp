#include "ColliderBox.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D 
{


ColliderBox::ColliderBox(): 
	_size(Vector3F::ONE)
{
	_shape = new btBoxShape(ToBtVector3(_size));
}

ColliderBox::~ColliderBox()
{
	SafeDelete(_shape);
}

void ColliderBox::RegisterObject()
{
	RegisterFactory<ColliderBox>();
}

void ColliderBox::SetSize(const Vector3F& vec)
{
	_size = vec;
	//Resize(vec);
}

void ColliderBox::SetSize(float x, float y, float z)
{
	_size = Vector3F(x, y, z);
	//Resize(Vector3(x, y, z));
}

void ColliderBox::SetSize(float scale)
{
	_size = Vector3F(scale, scale, scale);
	//Resize(Vector3(scale));
}


void ColliderBox::Resize(const Vector3F& vec)
{
	if (!_shape)
		return;
	SafeDelete(_shape);
	_shape = new btBoxShape(ToBtVector3(vec));
	//_shape->setLocalScaling(ToBtVector3(_cachedWorldScale));
}

}