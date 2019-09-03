#include "PhysicsWorld.h"
#include "PhysicsUtils.h"
#include "../RegisteredBox/RegisteredBox.h"
#include "../Scene/Scene.h"

namespace Auto3D 
{

const char* PHYSICS_CATEGORY = "Physics";
extern const char* SUBSYSTEM_CATEGORY;

static const int MAX_SOLVER_ITERATIONS = 256;
static const Vector3F DEFAULT_GRAVITY = Vector3F(0.0f, -9.81f, 0.0f);

PhysicsWorldConfig PhysicsWorld::config;

PhysicsWorld::PhysicsWorld():
		_fps(DEFAULT_FPS)
{
	_time = Subsystem<Time>();

	if (PhysicsWorld::config.collisionConfig)
		_collisionConfiguration = PhysicsWorld::config.collisionConfig;
	else
		_collisionConfiguration = new btDefaultCollisionConfiguration();

	_collisionDispatcher = new btCollisionDispatcher(_collisionConfiguration);
	_broadphase = new btDbvtBroadphase();
	_solver = new btSequentialImpulseConstraintSolver();
	_world = new btDiscreteDynamicsWorld(_collisionDispatcher, _broadphase, _solver, _collisionConfiguration);

	_world->setGravity(ToBtVector3(DEFAULT_GRAVITY));
	_world->getDispatchInfo().m_useContinuous = true;
	_world->getSolverInfo().m_splitImpulse = false; // Disable by default for performance
	//_world->setDebugDrawer(this);
	//_world->setInternalTickCallback(InternalPreTickCallback, static_cast<void*>(this), true);
	//_world->setInternalTickCallback(InternalTickCallback, static_cast<void*>(this), false);
	_world->setSynchronizeAllMotionStates(true);


	// Get active scene to set physics world
	Object::Subsystem<RegisteredBox>()->GetActiveScene()->SetPhysicsWorld(this);
}


PhysicsWorld::~PhysicsWorld()
{
	SafeDelete(_world);
	SafeDelete(_solver);
	SafeDelete(_broadphase);
	SafeDelete(_collisionDispatcher);
	if (!PhysicsWorld::config.collisionConfig)
		SafeDelete(_collisionConfiguration);
}

void PhysicsWorld::RegisterObject()
{
	RegisterFactory<PhysicsWorld>();
}

void PhysicsWorld::Update()
{
	float timeStep = 0.1f;
	SetFPS(1 / _time->GetDeltaTime());
	float internalTimeStep = 1.0f / _fps;
	int maxSubSteps = (int)(timeStep * _fps) + 1;

	if (maxSubSteps < 0)
	{
		internalTimeStep = timeStep;
		maxSubSteps = 1;
	}
	_world->stepSimulation(timeStep, maxSubSteps, internalTimeStep);
}
//
//void PhysicsWorld::AddRigidBody(SharedPtr<RigidBody> rigidBody)
//{
//	_rigidBodies.push_back(rigidBody);
//}
//
//void PhysicsWorld::RemoveRigidBody(SharedPtr<RigidBody> rigidBody)
//{
//	VectorFindEarse(_rigidBodies, rigidBody);
//}
//
//void PhysicsWorld::AddCollider(SharedPtr<Collider> collider)
//{
//	_colliders.push_back(collider);
//}
//
//void PhysicsWorld::RemoveCollider(SharedPtr<Collider> collider)
//{
//	VectorFindEarse(_colliders, collider);
//}
//
//void PhysicsWorld::AddConstraint(SharedPtr<Constraint> constraint)
//{
//	_constraints.push_back(constraint);
//}
//
//void PhysicsWorld::RemoveConstraint(SharedPtr<Constraint> constraint)
//{
//	VectorFindEarse(_constraints, constraint);
//}

void PhysicsWorld::SetFPS(int fps)
{
	_fps = (unsigned)Clamp(fps, 1, 1000);
}

void PhysicsWorld::DeleteColliders()
{
	//for (int j = 0; j < _colliders.size(); j++)
	//{
	//	btCollisionShape* shape = _colliders[j]->GetShape();
	//	//_colliders[j]->GetShape() = 0;
	//	delete shape;
	//}
}

}