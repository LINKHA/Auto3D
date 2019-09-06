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
	_isDirty(false),
	_inWorld(false),
	_body(nullptr)
{
	_compoundShape = new btCompoundShape();
}


RigidBody::~RigidBody()
{
	ReleaseBody();

	if (_physicsWorld)
		_physicsWorld->RemoveRigidBody(this);
}

void RigidBody::RegisterObject()
{
	RegisterFactory<RigidBody>();
}

void RigidBody::getWorldTransform(btTransform& worldTrans) const
{
	if (Parent())
	{
		SpatialNode* parentNode = dynamic_cast<SpatialNode*>(Parent());
		worldTrans.setOrigin(ToBtVector3(parentNode->GetPosition()));
		worldTrans.setRotation(ToBtQuaternion(parentNode->GetRotation()));
	}
}

void RigidBody::setWorldTransform(const btTransform& worldTrans)
{

	Quaternion newWorldRotation = BtToQuaternion(worldTrans.getRotation());
	Vector3F newWorldPosition = BtToVector3(worldTrans.getOrigin());
	RigidBody* parentRigidBody = nullptr;
	
	if (Parent())
	{
		SpatialNode* parentNode = dynamic_cast<SpatialNode*>(Parent());
		parentNode->SetPosition(newWorldPosition);
		parentNode->SetRotation(newWorldRotation);
	}
}

void RigidBody::UpdateMass()
{
	if (!_body)
		return;

	auto numShapes = (unsigned)_compoundShape->getNumChildShapes();
	if (!numShapes)
		return;

	if (numShapes && _physicsWorld)
	{
		_body->setCollisionShape(_compoundShape.Get());
	}

	btVector3 localInertia(0, 0, 0);
	if (_mass > 0.0f)
		_compoundShape->calculateLocalInertia(_mass, localInertia);

	_body->setMassProps(_mass, localInertia);
	_body->updateInertiaTensor();

	_physicsWorld->GetWorld()->removeRigidBody(_body);
	_physicsWorld->GetWorld()->addRigidBody(_body);
	
}

void RigidBody::SetMass(float mass)
{
	mass = Max(mass, 0.0f);

	if (mass != _mass)
	{
		_mass = mass;
		AddBodyToWorld();
	}
}

void RigidBody::ReleaseBody()
{
	if (_body)
	{
		RemoveBodyFromWorld();
	}
}

void RigidBody::ParentCallBack()
{
	_physicsWorld = ParentScene()->GetPhysicsWorld();

	_physicsWorld->AddRigidBody(this);

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
		RemoveBodyFromWorld();
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

	_inWorld = true;
}

void RigidBody::RemoveBodyFromWorld()
{
	if (_physicsWorld && _body && _inWorld)
	{
		btDiscreteDynamicsWorld* world = _physicsWorld->GetWorld();
		world->removeRigidBody(_body);
		_inWorld = false;
	}
}

}