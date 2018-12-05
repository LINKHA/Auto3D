#include "ColliderSphere.h"
#include "Ambient.h"
#include "PhysicsUtils.h"
#include "Transform.h"
#include "PhysicsWorld.h"
#include "MemoryDef.h"

namespace Auto3D {


ColliderSphere::ColliderSphere(Ambient* ambient)
	: Super(ambient)
	, _size(1.0f)
{
	
}


ColliderSphere::~ColliderSphere()
{
	SafeDelete(_shape);
}

void ColliderSphere::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<ColliderSphere>(SCENE_ATTACH);
}

void ColliderSphere::Start()
{
	_shape = new btSphereShape(_size);
	//_shape->setLocalScaling(ToBtVector3(_cachedWorldScale));
	Super::Start();
	//_physicsWorld->AddCollider(this);
}

void ColliderSphere::Update()
{

}

void ColliderSphere::SetSize(float size)
{
	_size = size;
	//resize(size);
}

void ColliderSphere::resize(float size)
{
	if (!_shape)
		return;
	SafeDelete(_shape);
	_shape = new btSphereShape(size);
	//_shape->setLocalScaling(ToBtVector3(_cachedWorldScale));
}

}