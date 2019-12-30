#include "RigidBody.h"
#include "Collider.h"
#include "PhysicsWorld.h"
#include "PhysicsUtils.h"

#include "Scene/Transform.h"
#include "Scene/World.h"


namespace Auto3D 
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(ARigidBody)
	.constructor<>()
	;
}

ARigidBody::ARigidBody() :
	_mass(0.0f),
	_isDynamic(false),
	_isDirty(false),
	_inWorld(false),
	_body(nullptr)
{
	_compoundShape = new btCompoundShape();
}


ARigidBody::~ARigidBody()
{
	ReleaseBody();

	if (_physicsWorld)
		_physicsWorld->RemoveRigidBody(this);
}

void ARigidBody::getWorldTransform(btTransform& worldTrans) const
{
	if (GetParentNode())
	{
		ATransform* parentNode = dynamic_cast<ATransform*>(GetParentNode());
		worldTrans.setOrigin(ToBtVector3(parentNode->GetPosition()));
		worldTrans.setRotation(ToBtQuaternion(parentNode->GetRotation()));
	}
}

void ARigidBody::setWorldTransform(const btTransform& worldTrans)
{

	FQuaternion newWorldRotation = BtToQuaternion(worldTrans.getRotation());
	TVector3F newWorldPosition = BtToVector3(worldTrans.getOrigin());
	ARigidBody* parentRigidBody = nullptr;
	
	if (GetParentNode())
	{
		ATransform* parentNode = dynamic_cast<ATransform*>(GetParentNode());
		parentNode->SetPosition(newWorldPosition);
		parentNode->SetRotation(newWorldRotation);
	}
}

void ARigidBody::UpdateMass()
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

void ARigidBody::SetMass(float mass)
{
	mass = Max(mass, 0.0f);

	if (mass != _mass)
	{
		_mass = mass;
		AddBodyToWorld();
	}
}

void ARigidBody::ReleaseBody()
{
	if (_body)
	{
		RemoveBodyFromWorld();
	}
}

void ARigidBody::OnWorldSet(AWorld* newWorld, AWorld* oldWorld)
{
	if (newWorld)
	{
		_physicsWorld = GetWorld()->GetPhysicsWorld();
		_physicsWorld->AddRigidBody(this);
		AddBodyToWorld();
	}
}

void ARigidBody::AddBodyToWorld()
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
		TVector<ACollider*> shapes;
		GetParentNode()->FindChildrenNode<ACollider>(shapes, false);
		for (auto it = shapes.Begin(); it != shapes.End(); ++it)
		{
			(*it)->NotifyRigidBody(false);
		}
	}

	UpdateMass();

	_inWorld = true;
}

void ARigidBody::RemoveBodyFromWorld()
{
	if (_physicsWorld && _body && _inWorld)
	{
		btDiscreteDynamicsWorld* world = _physicsWorld->GetWorld();
		world->removeRigidBody(_body);
		_inWorld = false;
	}
}

}