#include "Component/ColliderComponent.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/PhysicsUtils.h"

#include "Debug/Log.h"
#include "Component/TransformComponent.h"
#include "Gameplay/WorldContext.h"
#include "Gameplay/World.h"

namespace Auto3D {

AColliderComponent::AColliderComponent() :
	_cachedWorldScale(TVector3F::ONE),
	_shapeType(EShapeType::DEFAULT),
	_rigidBody(nullptr),
	_shape(nullptr)
{
}


AColliderComponent::~AColliderComponent()
{
}

void AColliderComponent::ReleaseShape()
{
	btCompoundShape* compound = GetParentCompoundShape();
	if (_shape && compound)
	{
		compound->removeChildShape(_shape);
		_rigidBody->UpdateMass();
	}
	if (_shape)
	{
		delete _shape;
		_shape = nullptr;
	}
}

void AColliderComponent::NotifyRigidBody(bool updateMass)
{
	// Get this component rigidBody
	btCompoundShape* compound = GetParentCompoundShape();
	AActor* owner = GetOwner();
	if (owner && _shape && compound)
	{
		// Remove the shape first to ensure it is not added twice
		compound->removeChildShape(_shape);

		ATransformComponent* transform = owner->GetTransform();
		btTransform offset;
		offset.setOrigin(ToBtVector3(transform->GetPosition()));
		offset.setRotation(ToBtQuaternion(transform->GetRotation()));

		// Offset will be replaced by default
		compound->addChildShape(btTransform::getIdentity()/*offset*/, _shape);

		// Finally tell the rigid body to update its mass
		if (updateMass)
			_rigidBody->UpdateMass();
	}
}


btCompoundShape* AColliderComponent::GetParentCompoundShape()
{
	if (!_rigidBody)
		_rigidBody = GetOwner()->FindComponent<ARigidBodyComponent>();

	return _rigidBody ? _rigidBody->GetCompoundShape() : nullptr;
}

}