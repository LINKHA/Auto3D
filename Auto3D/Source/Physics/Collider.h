#pragma once
#include "Scene/Node.h"
#include "RigidBody.h"

#include <LinearMath/btMotionState.h>

class btCompoundShape;
class btCollisionShape;
class btRigidBody;
class btTransform;

namespace Auto3D 
{

/// Collision shape type.
namespace EShapeType
{
	enum Type
	{
		DEFAULT = 0,
		BOX,
		SPHERE,
		STATICPLANE,
		CYLINDER,
		CAPSULE,
		CONE,
		TRIANGLEMESH,
		CONVEXHULL,
		TERRAIN,
		GIMPACTMESH
	};
};

class APhysicsWorld;

class ACollider : public ANode
{
	DECLARE_CLASS(ACollider, ANode)
public:
	/// Construct
	ACollider();
	/// Destructor
	virtual ~ACollider();

	/// Get shape
	btCollisionShape* GetShape() { return _shape.Get(); }
	/// Release the collision shape.
	void ReleaseShape();
	/// Update the new collision shape to the ARigidBody.
	void NotifyRigidBody(bool updateMass = true);
	/// This function is called when the parent node of this class is assigned.
	virtual void ParentCallBack() override;

protected:
	/// Find the parent rigid body component and return its compound collision shape.
	btCompoundShape* GetParentCompoundShape();
	/// APhysics world form this collider
	TSharedPtr<APhysicsWorld> _physicsWorld;
	/// Rigidbody form this collider
	TWeakPtr<ARigidBody> _rigidBody;
	/// Cached world scale.
	TVector3F _cachedWorldScale;
	/// Shape form this collider
	TUniquePtr<btCollisionShape> _shape;
	/// Shape type
	EShapeType::Type _shapeType;
};

}