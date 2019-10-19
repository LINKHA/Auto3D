#pragma once
#include "Node2D.h"
#include "../Time/Time.h"

#include <Box2D.h>

namespace Auto3D {

class RigidBody2D;

class PhysicsWorld2D : public Node2D
{
	REGISTER_OBJECT_CLASS(PhysicsWorld2D, Node2D)
public:
	/// Construct
	PhysicsWorld2D();
	/// Destructor
	~PhysicsWorld2D();
	/// Register factory and attributes.
	static void RegisterObject();

	/// Physics world update step simulation.
	void Update();
	/// Set fps
	void SetFPS(int fps);
	/// Return 2d dynamics world
	b2World* GetWorld() { return _world.Get(); }
	/// Add RigidBody
	void AddRigidBody(RigidBody2D* rigidbody);
	/// Remove collider
	void RemoveRigidBody(RigidBody2D* rigidbody);
	/// Get colliders with current physics world.
	Vector<RigidBody2D*> GetColliders() { return _rigidBody; }

	/// This function is called when the parent node of this class is assigned.
	virtual void ParentCallBack()override;
private:

	unsigned int _fps;
	/// Time system.
	WeakPtr<Time> _time;
	/// Box2D world.
	UniquePtr<b2World> _world;
	/// Collision shapes in the world
	Vector<RigidBody2D*> _rigidBody;
};


}