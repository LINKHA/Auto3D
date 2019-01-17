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
	/**
	* @brief : Construct
	*/
	explicit PhysicsWorld(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);
	/**
	* @brief : Override Update
	*/
	void Update()override;
	/**
	* @brief : Set fps
	*/
	void SetFPS(int fps);
	/**
	* @brief : 
	*/
	btDiscreteDynamicsWorld* GetWorld() { return _world; }
	/**
	* @brief :
	*/
	void AddRigidBody(SharedPtr<RigidBody> rigidBody);
	/**
	* @brief : Remove rigbody
	*/
	void RemoveRigidBody(SharedPtr<RigidBody> rigidBody);
	/**
	* @brief : Add collider
	*/
	void AddCollider(SharedPtr<Collider> collider);
	/**
	* @brief : Remove collider
	*/
	void RemoveCollider(SharedPtr<Collider> collider);
	/**
	* @brief : Add constraint
	*/
	void AddConstraint(SharedPtr<Constraint> constraint);
	/**
	* @brief : Remove constraint
	*/
	void RemoveConstraint(SharedPtr<Constraint> constraint);
	/**
	* @brief : Get rigbodies (VECTOR<SharedPtr<RigidBody> >)
	*/
	VECTOR<SharedPtr<RigidBody> > GetRigidBodies() { return _rigidBodies; }
	/**
	* @brief : Get colliders (VECTOR<SharedPtr<Collider> >)
	*/
	VECTOR<SharedPtr<Collider> > GetColliders() { return _colliders; }
	/**
	* @brief : Get constraints (VECTOR<SharedPtr<Constraint> >)
	*/
	VECTOR<SharedPtr<Constraint> > GetConstraints() { return _constraints; }
private:
	/** 
	* @brief : Delete collision shapes
	*/
	void deleteColliders();
public:
	/// Overrides of the internal configuration
	static struct PhysicsWorldConfig config;
private:
	/// FPS
	unsigned _fps{ DEFAULT_FPS };
	/// Time system
	SharedPtr<Time> time;
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
	/// RigidBody container
	VECTOR<SharedPtr<RigidBody> > _rigidBodies;
	/// Collision shapes in the world
	VECTOR<SharedPtr<Collider> > _colliders;
	/// Constraints in the world
	VECTOR<SharedPtr<Constraint> > _constraints;
};

}