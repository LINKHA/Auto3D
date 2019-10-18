#pragma once
#include "../Scene/Node.h"
#include "../Time/Time.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Auto3D {

class RigidBody;

struct PhysicsWorldConfig
{

	PhysicsWorldConfig() :
		collisionConfig(nullptr)
	{}
	/// Override for the collision configuration (default btDefaultCollisionConfiguration).
	btCollisionConfiguration* collisionConfig;
};

class PhysicsWorld : public Node//, public btIDebugDraw
{
	REGISTER_OBJECT_CLASS(PhysicsWorld, Node)
public:
	/// Construct
	PhysicsWorld();
	/// Destructor
	~PhysicsWorld();
	/// Register factory and attributes.
	static void RegisterObject();

	/// Physics world update step simulation.
	void Update();
	/// Set fps
	void SetFPS(int fps);
	/// Return 3d dynamics world
	btDiscreteDynamicsWorld* GetWorld() { return _world.Get(); }
	/// Add RigidBody
	void AddRigidBody(RigidBody* rigidbody);
	/// Remove collider
	void RemoveRigidBody(RigidBody* rigidbody);

	/// Get colliders with current physics world.
	Vector<RigidBody*> GetColliders() { return _rigidBody; }
	/// This function is called when the parent node of this class is assigned.
	virtual void ParentCallBack()override;
	/// Overrides of the internal configuration
	static struct PhysicsWorldConfig config;
private:
	/// Delete collision shapes
	void ClearColliders();
	/// FPS
	unsigned _fps;
	/// Maximum number of simulation substeps per frame. 0 (default) unlimited, or negative values for adaptive timestep.
	int _maxSubSteps;
	/// Time system.
	WeakPtr<Time> _time;
	/// Bullet collision configuration
	UniquePtr<btCollisionConfiguration> _collisionConfiguration;
	/// Bullet collision dispatcher
	UniquePtr<btDispatcher> _collisionDispatcher;
	/// Bullet collision broadphase
	UniquePtr<btBroadphaseInterface> _broadphase;
	/// Bullet constraint solver
	UniquePtr<btConstraintSolver> _solver;
	/// Bullet physics world
	UniquePtr<btDiscreteDynamicsWorld> _world;
	/// Collision shapes in the world
	Vector<RigidBody*> _rigidBody;
};

}