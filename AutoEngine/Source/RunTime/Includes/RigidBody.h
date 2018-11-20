#pragma once
#include "Component.h"
#include "AutoPhysics3D.h"

namespace Auto3D {



class RigidBody : public Component
{
	REGISTER_DERIVED_ABSTRACT_CLASS(RigidBody, Component);
	DECLARE_OBJECT_SERIALIZE(RigidBody);
public:
	explicit RigidBody(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);

	void Start()override;

	void Update()override;
	/**
	* @brief : Update mass and inertia to the Bullet rigid body. 
	*	Readd body to world if necessary: if was in world and the 
	*	Bullet collision shape to use changed.
	*/
	void UpdateMass();
	/**
	* @brief : Update gravity parameters to the Bullet rigid body
	*/
	void UpdateGravity();
	/**
	* @brief : Create the rigid body, or re-add to the physics world with changed flags. Calls UpdateMass().
	*/
	void AddBodyToWorld();

	void SetMass(float mass) { _mass = mass; }

	float GetMass() { return _mass; }
#if DebugCompoundShape
	btCompoundShape* GetCompoundShape() { return _compoundShape; }
#else
	btCollisionShape* GetCollisionShape() { return _shape; }
#endif
private:
	void registeredRigidBody();
private:
	/// physics world
	PhysicsWorld* _physicsWorld;
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
	/// motion state
	btDefaultMotionState* _motionState;
	/// rigidbody mass
	float _mass{};
	
	bool _isDynamic;

	bool _isFirstUpdate{};

};

}