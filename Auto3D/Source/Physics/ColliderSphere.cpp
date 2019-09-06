#include "ColliderSphere.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D
{

ColliderSphere::ColliderSphere() :
	_size(1.0f)
{
	_shapeType = ShapeType::SPHERE;
}

ColliderSphere::~ColliderSphere()
{
}

void ColliderSphere::RegisterObject()
{
	RegisterFactory<ColliderSphere>();
}


void ColliderSphere::SetSize(float scale)
{
	_size = scale;
	Resize(_size);
}


void ColliderSphere::Resize(float radius)
{
	ReleaseShape();
	_shape.Reset();
	_shape = new btSphereShape(radius);

	NotifyRigidBody();
}

}