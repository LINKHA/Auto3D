#include "Collider.h"
#include "PhysicsWorld.h"
#include "PhysicsUtils.h"

#include "../Debug/Log.h"
#include "../Scene/SpatialNode.h"
#include "../RegisteredBox/RegisteredBox.h"
#include "../Scene/Scene.h"

namespace Auto3D {

void tRigidBody::getWorldTransform(btTransform& worldTrans) const
{
	worldTrans.setOrigin(ToBtVector3(_postion));
	worldTrans.setRotation(ToBtQuaternion(_quaternion));
}

void tRigidBody::setWorldTransform(const btTransform& worldTrans)
{
	_quaternion = BtToQuaternion(worldTrans.getRotation());
	_postion = BtToVector3(worldTrans.getOrigin());

	tRigidBody* parentRigidBody = nullptr;
}

void tRigidBody::CreateRigidBody(float mass, const Vector3F& inertia, btCollisionShape* shape, const Vector3F& postion, const Quaternion& quaternion)
{
	bool isDynamic = (mass != 0.f);
	_isDynamic = isDynamic;
	_mass = mass;
	_inertia = inertia;
	_postion = postion;

	btVector3 localInertia = ToBtVector3(inertia);

	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	_body = new btRigidBody(mass, this, shape, localInertia);

	//add the body to the dynamics world
	_physicsWorld->GetWorld()->addRigidBody(_body);
}

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
//
//void Collider::SetRigidBody(float mass, const Vector3F& inertia)
//{
//	if (!_rigidBody)
//		_rigidBody = new tRigidBody(_physicsWorld);
//
//	SpatialNode* parentNode = dynamic_cast<SpatialNode*>(Parent());
//	if (!parentNode)
//	{
//		WarningString("Fail set rigidBody,Maybe parent node dont spatial node.");
//		return;
//	}
//	Vector3F parentPostion = parentNode->GetPosition();
//	Quaternion parentQuaternion = parentNode->GetRotation();
//
//	_rigidBody->CreateRigidBody(mass, inertia, _shape, parentPostion, parentQuaternion);
//}

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
		compound->addChildShape(offset, _shape.Get());

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