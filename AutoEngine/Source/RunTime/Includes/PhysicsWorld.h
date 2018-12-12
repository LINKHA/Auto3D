#pragma once
#include "Component.h"
#include "AutoPhysics3D.h"
#include "Time.h"
#include "Constraint.h"
#include "Collider.h"


namespace Auto3D {

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

class PhysicsWorld : public Component//, public btIDebugDraw
{
	REGISTER_OBJECT_CLASS(PhysicsWorld, Component)
public:
	explicit PhysicsWorld(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);


	void Update()override;
	
	void SetFPS(int fps);

	btDiscreteDynamicsWorld* GetWorld() { return _world; }

	void AddRigidBodies(RigidBody* rigidBody);

	void RemoveRigidBodies(RigidBody* rigidBody);

	void AddCollider(Collider* collider);

	void RemoveCollider(Collider* collider);

	void AddConstraint(Constraint* constraint);

	void RemoveConstraint(Constraint* constraint);

	VECTOR<RigidBody*> GetRigidBodies() { return _rigidBodies; }

	VECTOR<Collider*> GetColliders() { return _colliders; }

	VECTOR<Constraint*> GetConstraints() { return _constraints; }

	/// Overrides of the internal configuration.
	static struct PhysicsWorldConfig config;
private:
	//delete collision shapes
	void deleteColliders();
private:
	unsigned _fps{ DEFAULT_FPS };
	Time* time;
	/// Bullet collision configuration.
	btCollisionConfiguration* _collisionConfiguration{};
	/// Bullet collision dispatcher.
	btDispatcher* _collisionDispatcher;
	/// Bullet collision broadphase.
	btBroadphaseInterface* _broadphase;
	/// Bullet constraint solver.
	btConstraintSolver* _solver;
	/// Bullet physics world.
	btDiscreteDynamicsWorld* _world;

	VECTOR<RigidBody*> _rigidBodies;
	/// Collision shapes in the world.
	VECTOR<Collider*> _colliders;
	/// Constraints in the world.
	VECTOR<Constraint*> _constraints;
};

}