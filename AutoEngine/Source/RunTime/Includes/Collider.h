#pragma once
#include "Component.h"
#include "AutoPhysics3D.h"
#include "RigidBody.h"
namespace Auto3D {


/// Collision shape type.
enum class ShapeType
{
	SHAPE_BOX = 0,
	SHAPE_SPHERE,
	SHAPE_STATICPLANE,
	SHAPE_CYLINDER,
	SHAPE_CAPSULE,
	SHAPE_CONE,
	SHAPE_TRIANGLEMESH,
	SHAPE_CONVEXHULL,
	SHAPE_TERRAIN,
	SHAPE_GIMPACTMESH
};
class PhysicsWorld;

class Collider : public Component
{
	REGISTER_OBJECT_ABSTRACT_CLASS(Collider, Component)
public:
	/**
	* @brief : Construct
	*/
	explicit Collider(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);

	void Start()override;
	void Update()override;

	btCompoundShape* GetParentCompoundShape();
	
	void NotifyRigidBody();

	btCollisionShape* GetShape() { return _shape; }
protected:
	PhysicsWorld* _physicsWorld;
	/// 
	RigidBody* _rigidBody;
	/// Cached world scale.
	Vector3 _cachedWorldScale;

	btCollisionShape* _shape;

	ShapeType shapeType_;

};

}