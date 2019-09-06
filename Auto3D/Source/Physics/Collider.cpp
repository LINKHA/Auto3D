#include "Collider.h"
#include "PhysicsWorld.h"
#include "PhysicsUtils.h"

#include "../Debug/Log.h"
#include "../Scene/SpatialNode.h"
#include "../RegisteredBox/RegisteredBox.h"
#include "../Scene/Scene.h"

namespace Auto3D {

Collider::Collider() :
	_cachedWorldScale(Vector3F::ONE)
{
}


Collider::~Collider()
{
	if(_physicsWorld)
		_physicsWorld->RemoveCollider(this);
}

void Collider::RegisterObject()
{
	RegisterFactory<Collider>();
}

void Collider::SetPhysicsWroldPostion(const Vector3F& pos)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(0, -56, 0));
	
}

void Collider::NotifyRigidBody(bool updateMass)
{
	// Get this component rigidBody
	btCompoundShape* compound = GetParentCompoundShape();
	
	if (Parent() && _shape && compound)
	{
		// Remove the shape first to ensure it is not added twice
		compound->removeChildShape(_shape.Get());

		SpatialNode* parentNode = dynamic_cast<SpatialNode*>(Parent());
		btTransform offset;
		offset.setOrigin(ToBtVector3(parentNode->GetPosition()));
		offset.setRotation(ToBtQuaternion(parentNode->GetRotation()));

		// Offset will be replaced by default
		compound->addChildShape(btTransform::getIdentity()/*offset*/, _shape.Get());

		// Finally tell the rigid body to update its mass
		if (updateMass)
			_rigidBody->UpdateMass();
	}
}

void Collider::ParentCallBack()
{
	_physicsWorld = ParentScene()->GetPhysicsWorld();
	_physicsWorld->AddCollider(this);
}

btCompoundShape* Collider::GetParentCompoundShape()
{
	if (!_rigidBody)
		_rigidBody = Parent()->FindChild<RigidBody>();

	return _rigidBody ? _rigidBody->GetCompoundShape() : nullptr;
}

}