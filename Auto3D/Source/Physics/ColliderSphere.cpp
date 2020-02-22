#include "ColliderSphere.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D
{

AColliderSphere::AColliderSphere() :
	_size(1.0f)
{
	_shapeType = EShapeType::SPHERE;
}

AColliderSphere::~AColliderSphere()
{
}

void AColliderSphere::SetSize(float scale)
{
	_size = scale;
	Resize(_size);
}


void AColliderSphere::Resize(float radius)
{
	ReleaseShape();
	delete _shape;
	_shape = nullptr;
	_shape = new btSphereShape(radius);

	NotifyRigidBody();
}

}