#include "Collider.h"
#include "Ambient.h"
#include "PhysicsWorld.h"
#include "PhysicsUtils.h"
#include "Transform.h"
#include "GameObject.h"

namespace Auto3D {


Collider::Collider(Ambient* ambient)
	: Super(ambient)
	, _cachedWorldScale(Vector3::one)
{
	
}


Collider::~Collider()
{
	//if(_physicsWorld)
	//	_physicsWorld->RemoveCollider(this);
}

void Collider::RegisterObject(Ambient* ambient)
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
		_rigidBody = GetGameObject().GetComponent<RigidBody>();

	return _rigidBody ? _rigidBody->GetCompoundShape() : nullptr;
}

void Collider::NotifyRigidBody()
{
	btCompoundShape* compound = GetParentCompoundShape();
	if (_shape && compound)
	{
		compound->removeChildShape(_shape);
		Vector3 position = GetGameObject().GetComponent<Transform>()->GetPosition();
		btTransform offset;
		offset.setOrigin(ToBtVector3(position));
		compound->addChildShape(offset, _shape);
	}
}
#else
void Collider::NotifyRigidBody()
{
	GetGameObject().GetComponent<RigidBody>()->_shape = _shape;
}
#endif



}