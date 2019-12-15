#include "ColliderSphere.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D
{

	REGISTER_CLASS
{
	using namespace rttr;
	registration::class_<AColliderSphere>("ColliderSphere")
		.constructor<>()
		.property("size", &AColliderSphere::GetSize, &AColliderSphere::SetSize)
		(
			metadata(SERIALIZABLE, "")
		)
		;
}

AColliderSphere::AColliderSphere() :
	_size(1.0f)
{
	_shapeType = EShapeType::SPHERE;
}

AColliderSphere::~AColliderSphere()
{
}

void AColliderSphere::RegisterObject()
{
	RegisterFactory<AColliderSphere>();
}


void AColliderSphere::SetSize(float scale)
{
	_size = scale;
	Resize(_size);
}


void AColliderSphere::Resize(float radius)
{
	ReleaseShape();
	_shape.Reset();
	_shape = new btSphereShape(radius);

	NotifyRigidBody();
}

}