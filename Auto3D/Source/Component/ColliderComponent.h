#pragma once
#include "Component/ActorComponent.h"
#include "Math/Vector3.h"
#include "Component/RigidBodyComponent.h"

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

class FPhysicsWorld;

class AColliderComponent : public AActorComponent
{
	DECLARE_A_CLASS(AColliderComponent, AActorComponent)
public:
	/// Construct
	AColliderComponent();
	/// Destructor
	virtual ~AColliderComponent();

	/// Get shape
	btCollisionShape* GetShape() { return _shape; }
	/// Release the collision shape.
	void ReleaseShape();
	/// Update the new collision shape to the ARigidBody.
	void NotifyRigidBody(bool updateMass = true);

protected:
	/// Find the parent rigid body component and return its compound collision shape.
	btCompoundShape* GetParentCompoundShape();
	/// Rigidbody form this collider
	ARigidBodyComponent* _rigidBody;
	/// Cached world scale.
	TVector3F _cachedWorldScale;
	/// Shape form this collider
	btCollisionShape* _shape;
	/// Shape type
	EShapeType::Type _shapeType;
};

}