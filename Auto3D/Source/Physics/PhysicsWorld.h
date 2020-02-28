#pragma once
#include "AutoConfig.h"
#include "Container/Ptr.h"
#include "Time/Time.h"
#include "Container/Vector.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Auto3D {

class ARigidBodyComponent;

struct FPhysicsWorldConfig
{

	FPhysicsWorldConfig() :
		collisionConfig(nullptr)
	{}
	/// Override for the collision configuration (default btDefaultCollisionConfiguration).
	btCollisionConfiguration* collisionConfig;
};

class AUTO_API FPhysicsWorld//, public btIDebugDraw
{
public:
	/// Construct
	FPhysicsWorld();
	/// Destructor
	~FPhysicsWorld();

	/// APhysics world update step simulation.
	void Update();
	/// Set fps
	void SetFPS(int fps);
	/// Return 3d dynamics world
	btDiscreteDynamicsWorld* GetWorld() { return _world.Get(); }
	/// Add ARigidBody
	void AddRigidBody(ARigidBodyComponent* rigidbody);
	/// Remove collider
	void RemoveRigidBody(ARigidBodyComponent* rigidbody);

	/// Get colliders with current physics world.
	TVector<ARigidBodyComponent*> GetColliders() { return _rigidBody; }

	/// Overrides of the internal configuration
	static struct FPhysicsWorldConfig config;
private:
	/// Delete collision shapes
	void ClearColliders();
	/// FPS
	unsigned _fps;
	/// Maximum number of simulation substeps per frame. 0 (default) unlimited, or negative values for adaptive timestep.
	int _maxSubSteps;
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
	TVector<ARigidBodyComponent*> _rigidBody;
};

}