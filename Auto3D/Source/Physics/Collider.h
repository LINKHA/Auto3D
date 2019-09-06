#pragma once
#include "../Scene/Node.h"
#include "RigidBody.h"

#include <LinearMath/btMotionState.h>

class btCompoundShape;
class btCollisionShape;
class btRigidBody;
class btTransform;

namespace Auto3D 
{

/// Collision shape type.
namespace ShapeType
{
	enum Type
	{
		BOX = 0,
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

class PhysicsWorld;

class Collider : public Node
{
	REGISTER_OBJECT_CLASS(Collider, Node)
public:
	/// Construct
	Collider();
	/// Destructor
	virtual ~Collider();
	/// Register object factory.
	static void RegisterObject();

	/// Get shape
	btCollisionShape* GetShape() { return _shape.Get(); }
	/// Release the collision shape.
	void ReleaseShape();
	/// Update the new collision shape to the RigidBody.
	void NotifyRigidBody(bool updateMass = true);
	/// This function is called when the parent node of this class is assigned.
	virtual void ParentCallBack() override;

protected:
	/// Find the parent rigid body component and return its compound collision shape.
	btCompoundShape* GetParentCompoundShape();
	/// Physics world form this collider
	SharedPtr<PhysicsWorld> _physicsWorld;
	/// Rigidbody form this collider
	//SharedPtr<tRigidBody> _rigidBody;
	WeakPtr<RigidBody> _rigidBody;
	/// Cached world scale.
	Vector3F _cachedWorldScale;
	/// Shape form this collider
	UniquePtr<btCollisionShape> _shape;
	/// Shape type
	ShapeType::Type _shapeType;
};

}