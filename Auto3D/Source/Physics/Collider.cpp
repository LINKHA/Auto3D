#include "Collider.h"
#include "PhysicsWorld.h"
#include "PhysicsUtils.h"

#include "../RegisteredBox/RegisteredBox.h"
#include "../Scene/Scene.h"
#include "../Debug/DebugNew.h"

namespace Auto3D {


Collider::Collider() :
	_cachedWorldScale(Vector3F::ONE)
{
	//_physicsWorld->AddCollider(this);
	//NotifyRigidBody();
}


Collider::~Collider()
{
	//if(_physicsWorld)
	//	_physicsWorld->RemoveCollider(this);
	SafeDelete(_shape);
}

void Collider::RegisterObject()
{
	RegisterFactory<Collider>();
}

#if DebugCompoundShape
btCompoundShape* Collider::GetParentCompoundShape()
{
	if (!_rigidBody)
		_rigidBody = GetNode()->GetComponent<RigidBody>();

	return _rigidBody ? _rigidBody->GetCompoundShape() : nullptr;
}

void Collider::NotifyRigidBody()
{
	btCompoundShape* compound = GetParentCompoundShape();
	if (_shape && compound)
	{
		compound->removeChildShape(_shape);
		Vector3 position = GetNode()->GetComponent<Transform>()->GetPosition();
		btTransform offset;
		offset.setOrigin(ToBtVector3(position));
		compound->addChildShape(offset, _shape);
	}
}
#else
void Collider::NotifyRigidBody()
{
	//GetNode()->GetComponent<RigidBody>()->_shape = _shape;
}
#endif

void Collider::ParentCallBack()
{
	_physicsWorld = ParentScene()->GetPhysicsWorld();
	//_physicsWorld->AddCollider(this);
}

}