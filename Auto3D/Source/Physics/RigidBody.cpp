#include "RigidBody.h"
#include "Collider.h"
#include "PhysicsWorld.h"
#include "PhysicsUtils.h"

#include "../Scene/SpatialNode.h"
#include "../Scene/Scene.h"


namespace Auto3D 
{

RigidBody::RigidBody() :
	_mass(0.0f),
	_isDynamic(false),
	_isDirty(false)
{
	_compoundShape = new btCompoundShape();
}


RigidBody::~RigidBody()
{
}

void RigidBody::RegisterObject()
{
	RegisterFactory<RigidBody>();
}

void RigidBody::getWorldTransform(btTransform& worldTrans) const
{
	if (Parent())
	{
		worldTrans.setOrigin(ToBtVector3(dynamic_cast<SpatialNode*>(Parent())->GetPosition()));
		worldTrans.setRotation(ToBtQuaternion(dynamic_cast<SpatialNode*>(Parent())->GetRotation()));
	}
}

void RigidBody::setWorldTransform(const btTransform& worldTrans)
{

	Quaternion newWorldRotation = BtToQuaternion(worldTrans.getRotation());
	Vector3F newWorldPosition = BtToVector3(worldTrans.getOrigin());
	RigidBody* parentRigidBody = nullptr;
	
	if (Parent())
	{

	}
}

void RigidBody::UpdateMass()
{
	if (!_body)
		return;

	auto numShapes = (unsigned)_compoundShape->getNumChildShapes();
	if (numShapes && _physicsWorld)
	{
		_body->setCollisionShape(_compoundShape.Get());
		
		_physicsWorld->GetWorld()->removeRigidBody(_body.Get());
		_physicsWorld->GetWorld()->addRigidBody(_body.Get());
	}
	if (_mass != 0)
	{
		btVector3 localInertia(0, 0, 0);
		_compoundShape->calculateLocalInertia(_mass, localInertia);
	}
}

void RigidBody::ParentCallBack()
{
	_physicsWorld = ParentScene()->GetPhysicsWorld();

	AddBodyToWorld();
}

void RigidBody::AddBodyToWorld()
{
	if (!_physicsWorld)
		return;

	if (_mass < 0.0f)
		_mass = 0.0f;

	if (_body)
	{
		//RemoveBody
	}
	else
	{
		// Correct inertia will be calculated below
		btVector3 localInertia(0.0f, 0.0f, 0.0f);
		_body = new btRigidBody(_mass, this, _compoundShape.Get(), localInertia);
		_body->setUserPointer(this);

		// Check if CollisionShapes already exist in the node and add them to the compound shape.
		// Do not update mass yet, but do it once all shapes have been added.
		Vector<Collider*> shapes;
		Parent()->FindChildren<Collider>(shapes, false);
		for (auto it = shapes.Begin(); it != shapes.End(); ++it)
		{
			(*it)->NotifyRigidBody(false);
		}
	}

	UpdateMass();
}

}