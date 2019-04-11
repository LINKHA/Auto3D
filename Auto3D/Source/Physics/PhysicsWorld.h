#pragma once
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "../Scene/Node.h"
#include "../Time/Time.h"

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
	/// Set fps
	void SetFPS(int fps);
	/// Return 3d dynamics world
	btDiscreteDynamicsWorld* GetWorld() { return _world; }
	/*///
	void AddRigidBody(SharedPtr<RigidBody> rigidBody);
	/// Remove rigbody
	void RemoveRigidBody(SharedPtr<RigidBody> rigidBody);
	/// Add collider
	void AddCollider(SharedPtr<Collider> collider);
	/// Remove collider
	void RemoveCollider(SharedPtr<Collider> collider);
	/// Add constraint
	void AddConstraint(SharedPtr<Constraint> constraint);
	/// Remove constraint
	void RemoveConstraint(SharedPtr<Constraint> constraint);
	/// Get rigbodies (VECTOR<SharedPtr<RigidBody> >)
	Vector<RigidBody*> GetRigidBodies() { return _rigidBodies; }
	/// Get colliders (VECTOR<SharedPtr<Collider> >)
	Vector<Collider*> GetColliders() { return _colliders; }
	/// Get constraints (VECTOR<SharedPtr<Constraint> >)
	Vector<Constraint*> GetConstraints() { return _constraints; }*/
private:
	/**
	/// Delete collision shapes
	*/
	void deleteColliders();
public:
	/// Overrides of the internal configuration
	static struct PhysicsWorldConfig config;
private:
	/// FPS
	unsigned _fps{ DEFAULT_FPS };
	/// Time system
	AutoPtr<Time> time;
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
	///// RigidBody container
	//Vector<SharedPtr<RigidBody> > _rigidBodies;
	///// Collision shapes in the world
	//Vector<SharedPtr<Collider> > _colliders;
	///// Constraints in the world
	//Vector<SharedPtr<Constraint> > _constraints;
};

}