#pragma once
#include "../Scene/Node.h"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

namespace Auto3D 
{

class PhysicsWorld;

class RigidBody : public Node
{
	REGISTER_OBJECT_CLASS(RigidBody, Node)
public:
	/// Construct
	RigidBody();
	/// Destructor
	~RigidBody();
	/// Register object factory.
	static void RegisterObject();
	/// Update mass and inertia to the Bullet rigid body.Readd body to world if necessary: if was in world and the. Bullet collision shape to use changed.
	void UpdateMass();
	/// Update gravity parameters to the Bullet rigid body
	void UpdateGravity();
	/// Create the rigid body, or re-add to the physics world with changed flags. Calls UpdateMass().
	void AddBodyToWorld();
	/// Set mass
	void SetMass(float mass) { _mass = mass; }
	/// Get mass
	float GetMass() { return _mass; }
#if DebugCompoundShape
	btCompoundShape* GetCompoundShape() { return _compoundShape; }
#else
	btCollisionShape* GetCollisionShape() { return _shape; }
#endif
private:
	/// Register rigidbody
	void RegisteredRigidBody();

	/// physics world
	SharedPtr<PhysicsWorld> _physicsWorld;
	/// bullet rigid body
	btRigidBody* _body;
#if DebugCompoundShape
	/// bullet compound collision shape.
	btCompoundShape* _compoundShape;
#else
public:
	btCollisionShape* _shape;
private:
#endif
	/// Motion state
	btDefaultMotionState* _motionState;
	/// Rigidbody mass
	float _mass{};
	/// Is dynamic ot staitc
	bool _isDynamic;
	/// First frame didn't calculate physics
	bool _isFirstUpdate{};

};

}