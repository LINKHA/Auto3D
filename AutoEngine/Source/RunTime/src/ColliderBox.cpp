#include "ColliderBox.h"
#include "Ambient.h"
#include "PhysicsUtils.h"
#include "Transform.h"
#include "PhysicsWorld.h"
#include "MemoryDef.h"

namespace Auto3D {


ColliderBox::ColliderBox(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _size(Vector3::one)
{
	
}


ColliderBox::~ColliderBox()
{
	SafeDelete(_shape);
}

void ColliderBox::RegisterObject(SharedPtr<Ambient> ambient)
{
	ambient->RegisterFactory<ColliderBox>(SCENE_ATTACH);
}

void ColliderBox::Start()
{
	_shape = new btBoxShape(ToBtVector3(_size));
	//_shape->setLocalScaling(ToBtVector3(_cachedWorldScale));
	Super::Start();
	//_physicsWorld->AddCollider(this);
}

void ColliderBox::Update()
{

}

void ColliderBox::SetSize(const Vector3& vec)
{
	_size = vec;
	//resize(vec);
}

void ColliderBox::SetSize(float x, float y, float z)
{
	_size = Vector3(x, y, z);
	//resize(Vector3(x, y, z));
}

void ColliderBox::SetSize(float scale)
{
	_size = Vector3(scale);
	//resize(Vector3(scale));
}


void ColliderBox::resize(const Vector3& vec)
{
	if (!_shape)
		return;
	SafeDelete(_shape);
	_shape = new btBoxShape(ToBtVector3(vec));
	//_shape->setLocalScaling(ToBtVector3(_cachedWorldScale));
}

}