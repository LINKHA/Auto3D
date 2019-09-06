#include "ColliderSphere.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D
{

ColliderSphere::ColliderSphere() :
	_size(1.0f)
{
	_shapeType = ShapeType::SPHERE;
	_shape = new btSphereShape(_size);
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


void ColliderSphere::Resize(float vec)
{
	if (!_shape)
		return;
	btSphereShape* shape = dynamic_cast<btSphereShape*>(_shape.Get());

	//Resize the edge
	shape->setMargin(vec);

	NotifyRigidBody();
}

}