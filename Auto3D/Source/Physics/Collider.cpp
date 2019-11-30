#include "Collider.h"
#include "PhysicsWorld.h"
#include "PhysicsUtils.h"

#include "Debug/Log.h"
#include "Scene/SpatialNode.h"
#include "RegisteredBox/RegisteredBox.h"
#include "Scene/Scene.h"

namespace Auto3D {

ACollider::ACollider() :
	_cachedWorldScale(TVector3F::ONE),
	_shapeType(EShapeType::DEFAULT)
{
}


ACollider::~ACollider()
{
}

void ACollider::RegisterObject()
{
	RegisterFactory<ACollider>();
}

void ACollider::ReleaseShape()
{
	btCompoundShape* compound = GetParentCompoundShape();
	if (_shape && compound)
	{
		compound->removeChildShape(_shape.Get());
		_rigidBody->UpdateMass();
	}
	_shape.Reset();
}

void ACollider::NotifyRigidBody(bool updateMass)
{
	// Get this component rigidBody
	btCompoundShape* compound = GetParentCompoundShape();
	
	if (Parent() && _shape && compound)
	{
		// Remove the shape first to ensure it is not added twice
		compound->removeChildShape(_shape.Get());

		ASpatialNode* parentNode = dynamic_cast<ASpatialNode*>(Parent());
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

void ACollider::ParentCallBack()
{
	_physicsWorld = ParentScene()->GetPhysicsWorld();
}

btCompoundShape* ACollider::GetParentCompoundShape()
{
	if (!_rigidBody)
		_rigidBody = Parent()->FindChild<ARigidBody>();

	return _rigidBody ? _rigidBody->GetCompoundShape() : nullptr;
}

}