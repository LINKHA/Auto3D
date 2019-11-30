#pragma once
#include "Scene/Node.h"
#include "Time/Time.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Auto3D {

class ARigidBody;

struct PhysicsWorldConfig
{

	PhysicsWorldConfig() :
		collisionConfig(nullptr)
	{}
	/// Override for the collision configuration (default btDefaultCollisionConfiguration).
	btCollisionConfiguration* collisionConfig;
};

class APhysicsWorld : public ANode//, public btIDebugDraw
{
	REGISTER_OBJECT_CLASS(APhysicsWorld, ANode)
public:
	/// Construct
	APhysicsWorld();
	/// Destructor
	~APhysicsWorld();
	/// Register factory and attributes.
	static void RegisterObject();

	/// APhysics world update step simulation.
	void Update();
	/// Set fps
	void SetFPS(int fps);
	/// Return 3d dynamics world
	btDiscreteDynamicsWorld* GetWorld() { return _world.Get(); }
	/// Add ARigidBody
	void AddRigidBody(ARigidBody* rigidbody);
	/// Remove collider
	void RemoveRigidBody(ARigidBody* rigidbody);

	/// Get colliders with current physics world.
	TVector<ARigidBody*> GetColliders() { return _rigidBody; }
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
	/// ATime system.
	TWeakPtr<ATime> _time;
	/// Bullet collision configuration
	TUniquePtr<btCollisionConfiguration> _collisionConfiguration;
	/// Bullet collision dispatcher
	TUniquePtr<btDispatcher> _collisionDispatcher;
	/// Bullet collision broadphase
	TUniquePtr<btBroadphaseInterface> _broadphase;
	/// Bullet constraint solver
	TUniquePtr<btConstraintSolver> _solver;
	/// Bullet physics world
	TUniquePtr<btDiscreteDynamicsWorld> _world;
	/// Collision shapes in the world
	TVector<ARigidBody*> _rigidBody;
};

}