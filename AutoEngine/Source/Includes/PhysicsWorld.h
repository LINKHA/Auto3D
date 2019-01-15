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
	explicit PhysicsWorld(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);


	void Update()override;
	
	void SetFPS(int fps);

	btDiscreteDynamicsWorld* GetWorld() { return _world; }

	void AddRigidBodies(SharedPtr<RigidBody> rigidBody);

	void RemoveRigidBodies(SharedPtr<RigidBody> rigidBody);

	void AddCollider(SharedPtr<Collider> collider);

	void RemoveCollider(SharedPtr<Collider> collider);

	void AddConstraint(SharedPtr<Constraint> constraint);

	void RemoveConstraint(SharedPtr<Constraint> constraint);

	VECTOR<SharedPtr<RigidBody> > GetRigidBodies() { return _rigidBodies; }

	VECTOR<SharedPtr<Collider> > GetColliders() { return _colliders; }

	VECTOR<SharedPtr<Constraint> > GetConstraints() { return _constraints; }

	/// Overrides of the internal configuration.
	static struct PhysicsWorldConfig config;
private:
	//delete collision shapes
	void deleteColliders();
private:
	unsigned _fps{ DEFAULT_FPS };
	SharedPtr<Time> time;
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

	VECTOR<SharedPtr<RigidBody> > _rigidBodies;
	/// Collision shapes in the world.
	VECTOR<SharedPtr<Collider> > _colliders;
	/// Constraints in the world.
	VECTOR<SharedPtr<Constraint> > _constraints;
};

}