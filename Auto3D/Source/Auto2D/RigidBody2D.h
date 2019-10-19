#pragma once
#include "Node2D.h"
#include <Box2D.h>

namespace Auto3D {

class PhysicsWorld2D;

class AUTO_API RigidBody2D : public Node2D
{
	REGISTER_OBJECT_CLASS(RigidBody2D, Node2D)
public:
	/// Construct
	RigidBody2D();
	/// Destructor
	~RigidBody2D();
	/// Register object factory.
	static void RegisterObject();

private:
	/// Create the rigid body, or re-add to the physics world with changed flags. Calls UpdateMass().
	void AddBodyToWorld();
	/// Remove the rigid body from the physics world.
	void RemoveBodyFromWorld();

	/// Physics world 2d.
	WeakPtr<PhysicsWorld2D> _physicsWorld2d;
	/// Box2D body define.
	b2BodyDef _bodyDef;
	/// Box2D body.
	b2Body* _body;
};

}