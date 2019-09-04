#pragma once
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "../Scene/Node.h"
#include "../Time/Time.h"

namespace Auto3D {

class Collider;

struct PhysicsWorldConfig
{

	PhysicsWorldConfig() :
		collisionConfig(nullptr)
	{}
	/// Override for the collision configuration (default btDefaultCollisionConfiguration).
	btCollisionConfiguration* collisionConfig;
};

static const int DEFAULT_FPS = 60;
static const float DEFAULT_MAX_NETWORK_ANGULAR_VELOCITY = 100.0f;

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

	void Update();
	/// Set fps
	void SetFPS(int fps);
	/// Return 3d dynamics world
	btDiscreteDynamicsWorld* GetWorld() { return _world; }
	/// Add collider
	void AddCollider(Collider* collider);
	/// Remove collider
	void RemoveCollider(Collider* collider);

	/// Get colliders with current physics world.
	Vector<Collider*> GetColliders() { return _colliders; }

	virtual void ParentCallBack()override;
	/// Overrides of the internal configuration
	static struct PhysicsWorldConfig config;
private:
	/// Delete collision shapes
	void ClearColliders();
	/// FPS
	unsigned _fps{ DEFAULT_FPS };
	/// Maximum number of simulation substeps per frame. 0 (default) unlimited, or negative values for adaptive timestep.
	int _maxSubSteps;
	/// Time system
	WeakPtr<Time> _time;
	/// Bullet collision configuration
	btCollisionConfiguration* _collisionConfiguration{};
	/// Bullet collision dispatcher
	btDispatcher* _collisionDispatcher;
	/// Bullet collision broadphase
	btBroadphaseInterface* _broadphase;
	/// Bullet constraint solver
	btConstraintSolver* _solver;
	/// Bullet physics world
	btDiscreteDynamicsWorld* _world;
	/// Collision shapes in the world
	Vector<Collider*> _colliders;
};

}