#pragma once
#include "../Scene/Node.h"

#include "RigidBody.h"

namespace Auto3D 
{

/// Collision shape type.
namespace ShapeType
{
	enum Type
	{
		Box = 0,
		Sphere,
		Staticplane,
		Cylinder,
		Capsule,
		Cone,
		Trianglemesh,
		Convexhull,
		Terrain,
		Gimpactmesh
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
	/// Get parent compound shape
	btCompoundShape* GetParentCompoundShape();
	/// Set rigidBody shape
	void NotifyRigidBody();
	/// Get shape
	btCollisionShape* GetShape() { return _shape; }
protected:
	/// Physics world form this collider
	SharedPtr<PhysicsWorld> _physicsWorld;
	/// Rigidbody form this collider
	SharedPtr<RigidBody> _rigidBody;
	/// Cached world scale.
	Vector3F _cachedWorldScale;
	/// Shape form this collider
	btCollisionShape* _shape;
	/// Shape type
	ShapeType::Type _shapeType;
};

}