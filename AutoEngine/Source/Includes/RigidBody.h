#pragma once
#include "Component.h"
#include "AutoPhysics3D.h"

namespace Auto3D {

class PhysicsWorld;

class RigidBody : public Component
{
	REGISTER_OBJECT_ABSTRACT_CLASS(RigidBody, Component)
public:
	/**
	* @brief : Construct
	*/
	explicit RigidBody(SharedPtr<Ambient> ambient);
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
	/**
	* @brief : Set mass
	*/
	void SetMass(float mass) { _mass = mass; }
	/**
	* @brief : Get mass
	*/
	float GetMass() { return _mass; }
#if DebugCompoundShape
	btCompoundShape* GetCompoundShape() { return _compoundShape; }
#else
	btCollisionShape* GetCollisionShape() { return _shape; }
#endif
private:
	/**
	* @brief : Register rigidbody
	*/
	void registeredRigidBody();
private:
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