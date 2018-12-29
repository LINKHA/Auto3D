#include "Collider.h"
#include "Ambient.h"
#include "PhysicsWorld.h"
#include "PhysicsUtils.h"
#include "Transform.h"
#include "NewDef.h"

namespace Auto3D {


Collider::Collider(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _cachedWorldScale(Vector3::one)
{
	
}


Collider::~Collider()
{
	//if(_physicsWorld)
	//	_physicsWorld->RemoveCollider(this);
}

void Collider::RegisterObject(SharedPtr<Ambient> ambient)
{
	ambient->RegisterFactory<Collider>(SCENE_ATTACH);
}

void Collider::Start()
{
	_physicsWorld = GetCurrentSceneNode()->GetPhysicsWorld();
	//_physicsWorld->AddCollider(this);
	NotifyRigidBody();
}

void Collider::Update()
{
	
}
#if DebugCompoundShape
btCompoundShape* Collider::GetParentCompoundShape()
{
	if (!_rigidBody)
		_rigidBody = GetNode().GetComponent<RigidBody>();

	return _rigidBody ? _rigidBody->GetCompoundShape() : nullptr;
}

void Collider::NotifyRigidBody()
{
	btCompoundShape* compound = GetParentCompoundShape();
	if (_shape && compound)
	{
		compound->removeChildShape(_shape);
		Vector3 position = GetNode().GetComponent<Transform>()->GetPosition();
		btTransform offset;
		offset.setOrigin(ToBtVector3(position));
		compound->addChildShape(offset, _shape);
	}
}
#else
void Collider::NotifyRigidBody()
{
	GetNode().GetComponent<RigidBody>()->_shape = _shape;
}
#endif



}