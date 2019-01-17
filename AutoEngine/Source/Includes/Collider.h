#pragma once
#include "Component.h"
#include "AutoPhysics3D.h"
#include "RigidBody.h"
namespace Auto3D {


/// Collision shape type.
enum class ShapeType
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
class PhysicsWorld;

class Collider : public Component
{
	REGISTER_OBJECT_ABSTRACT_CLASS(Collider, Component)
public:
	/**
	* @brief : Construct
	*/
	explicit Collider(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);
	/**
	* @brief : Override Start
	*/
	void Start()override;
	/**
	* @brief : Override Update
	*/
	void Update()override;
	/**
	* @brief : Get parent compound shape
	*/
	btCompoundShape* GetParentCompoundShape();
	/**
	* @brief : Set rigidBody shape
	*/
	void NotifyRigidBody();
	/**
	* @brief : Get shape
	*/
	btCollisionShape* GetShape() { return _shape; }

protected:
	/// Physics world form this collider
	SharedPtr<PhysicsWorld> _physicsWorld;
	/// Rigidbody form this collider
	SharedPtr<RigidBody> _rigidBody;
	/// Cached world scale.
	Vector3 _cachedWorldScale;
	/// Shape form this collider
	btCollisionShape* _shape;
	/// Shape type
	ShapeType _shapeType;

};

}