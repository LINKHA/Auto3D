#include "Collider.h"
#include "PhysicsWorld.h"
#include "PhysicsUtils.h"

#include "Debug/Log.h"
#include "Component/TransformComponent.h"
#include "Gameplay/WorldContext.h"
#include "Gameplay/World.h"

namespace Auto3D {

ACollider::ACollider() :
	_cachedWorldScale(TVector3F::ONE),
	_shapeType(EShapeType::DEFAULT)
{
}


ACollider::~ACollider()
{
}

void ACollider::ReleaseShape()
{
	btCompoundShape* compound = GetParentCompoundShape();
	if (_shape && compound)
	{
		compound->removeChildShape(_shape);
		_rigidBody->UpdateMass();
	}
	delete _shape;
	_shape = nullptr;
}

void ACollider::NotifyRigidBody(bool updateMass)
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


btCompoundShape* ACollider::GetParentCompoundShape()
{
	if (!_rigidBody)
		_rigidBody = GetOwner()->FindComponent<ARigidBody>();

	return _rigidBody ? _rigidBody->GetCompoundShape() : nullptr;
}

}