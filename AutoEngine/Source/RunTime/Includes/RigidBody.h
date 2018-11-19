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
	/**
	* @brief : Create the rigid body, or re-add to the physics world with changed flags. Calls UpdateMass().
	*/
	void AddBodyToWorld();
private:
	/// Physics world
	PhysicsWorld* _physicsWorld;
	/// Bullet rigid body
	btRigidBody* _body;
	/// Bullet compound collision shape.
	btCompoundShape* _compoundShape;
	/// Compound collision shape with center of mass offset applied.
	btCompoundShape* _shiftedCompoundShape;
};

}