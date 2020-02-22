#include "RigidBody.h"
#include "Collider.h"
#include "PhysicsWorld.h"
#include "PhysicsUtils.h"

#include "Component/TransformComponent.h"
#include "Gameplay/World.h"


namespace Auto3D 
{

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
	FPhysicsWorld* physicsWorld = GetWorld()->GetPhysicsWorld();
	if (physicsWorld)
		physicsWorld->RemoveRigidBody(this);
}

void ARigidBody::getWorldTransform(btTransform& worldTrans) const
{
	AActor* owner = GetOwner();
	if (owner)
	{
		ATransformComponent* transform = owner->GetTransform();
		worldTrans.setOrigin(ToBtVector3(transform->GetPosition()));
		worldTrans.setRotation(ToBtQuaternion(transform->GetRotation()));
	}
}

void ARigidBody::setWorldTransform(const btTransform& worldTrans)
{

	FQuaternion newWorldRotation = BtToQuaternion(worldTrans.getRotation());
	TVector3F newWorldPosition = BtToVector3(worldTrans.getOrigin());
	ARigidBody* parentRigidBody = nullptr;
	AActor* owner = GetOwner();

	if (owner)
	{
		ATransformComponent* transform = owner->GetTransform();
		transform->SetPosition(newWorldPosition);
		transform->SetRotation(newWorldRotation);
	}
}

void ARigidBody::UpdateMass()
{
	if (!_body)
		return;

	auto numShapes = (unsigned)_compoundShape->getNumChildShapes();
	if (!numShapes)
		return;
	FPhysicsWorld* physicsWorld = GetWorld()->GetPhysicsWorld();

	if (numShapes && physicsWorld)
	{
		_body->setCollisionShape(_compoundShape);
	}

	btVector3 localInertia(0, 0, 0);
	if (_mass > 0.0f)
		_compoundShape->calculateLocalInertia(_mass, localInertia);

	_body->setMassProps(_mass, localInertia);
	_body->updateInertiaTensor();

	physicsWorld->GetWorld()->removeRigidBody(_body);
	physicsWorld->GetWorld()->addRigidBody(_body);
	
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

//void ARigidBody::OnWorldSet(AWorld* newWorld, AWorld* oldWorld)
void ARigidBody::OnActorSet(AActor* newParent, AActor* oldParent)
{
	Super::OnActorSet(newParent, oldParent);
	if (newParent)
	{
		GetWorld()->GetPhysicsWorld()->AddRigidBody(this);
		AddBodyToWorld();
	}
}

void ARigidBody::AddBodyToWorld()
{
	FPhysicsWorld* physicsWorld = GetWorld()->GetPhysicsWorld();

	if (!physicsWorld)
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

		_body = new btRigidBody(_mass, this, _compoundShape, localInertia);
		
		_body->setUserPointer(this);

		// Check if CollisionShapes already exist in the node and add them to the compound shape.
		// Do not update mass yet, but do it once all shapes have been added.
		TVector<ACollider*> shapes;
		GetOwner()->FindComponents<ACollider>(shapes, false);
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
	if (_body && _inWorld)
	{
		FPhysicsWorld* physicsWorld = GetOwner()->GetWorld()->GetPhysicsWorld();
		btDiscreteDynamicsWorld* world = physicsWorld->GetWorld();
		world->removeRigidBody(_body);
		_inWorld = false;
	}
}

}