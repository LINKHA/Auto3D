#pragma once
#include "Component.h"
#include "AutoPhysics2D.h"

namespace Auto3D {

class Collider2D;
class PhysicsWorld2D;


/// Rigid body type.
enum class BodyType2D
{
	STATIC = b2_staticBody,
	KINEMATIC = b2_kinematicBody,
	DYNAMIC = b2_dynamicBody
};

class RigidBody2D : public Component
{
	REGISTER_DERIVED_ABSTRACT_CLASS(RigidBody2D, Component);
	DECLARE_OBJECT_SERIALIZE(RigidBody2D);
public:
	explicit RigidBody2D(Ambient* ambient);
	
	void OnSceneSet(PhysicsWorld2D* physicsWorld);

	/// Set body type.
	void SetBodyType(BodyType2D type);
	/// Set mass.
	void SetMass(float mass);
	/// Set inertia.
	void SetInertia(float inertia);


private:
	PhysicsWorld2D* _physicsWorld;
	/// Box2D body define.
	b2BodyDef _bodyDef;
	/// Box2D body.
	b2Body* _body;
	/// Box2D mass data.
	b2MassData _massData;
	/// Use fixture mass (calculate mass & inertia from collision shapes automatically.)
	bool _useFixtureMass;
};

}