#pragma once
#include "Node2D.h"
#include "Time/Time.h"

#include <Box2D.h>

namespace Auto3D {

class ARigidBody2D;

class APhysicsWorld2D : public ANode2D, public b2ContactListener
{
	DECLARE_CLASS(APhysicsWorld2D, ANode2D)
public:
	/// Construct
	APhysicsWorld2D();
	/// Destructor
	~APhysicsWorld2D();
	/// Register factory and attributes.
	static void RegisterObject();

	/// APhysics world update step simulation.
	void Update();
	/// Set fps
	void SetFPS(int fps);
	/// Return 2d dynamics world
	b2World* GetWorld() { return _world.Get(); }
	/// Add ARigidBody
	void AddRigidBody(ARigidBody2D* rigidbody);
	/// Remove collider
	void RemoveRigidBody(ARigidBody2D* rigidbody);
	/// Get colliders with current physics world.
	TVector<ARigidBody2D*> GetColliders() { return _rigidBodies; }

	/// This function is called when the parent node of this class is assigned.
	virtual void ParentCallBack()override;
private:

	unsigned int _fps;
	/// ATime system.
	TWeakPtr<FTimeModule> _time;
	/// Box2D world.
	TUniquePtr<b2World> _world;
	/// Collision shapes in the world
	TVector<ARigidBody2D*> _rigidBodies;

	/// Gravity.
	TVector2F _gravity;
	/// Velocity iterations.
	int _velocityIterations{};
	/// Position iterations.
	int _positionIterations{};
};


}