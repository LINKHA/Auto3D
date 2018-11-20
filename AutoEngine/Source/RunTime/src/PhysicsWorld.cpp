#include "PhysicsWorld.h"
#include "Ambient.h"
#include "PhysicsUtils.h"
#include "Collider.h"

namespace Auto3D {
const char* PHYSICS_CATEGORY = "Physics";
extern const char* SUBSYSTEM_CATEGORY;

static const int MAX_SOLVER_ITERATIONS = 256;
static const Vector3 DEFAULT_GRAVITY = Vector3(0.0f, -9.81f, 0.0f);

PhysicsWorldConfig PhysicsWorld::config;

PhysicsWorld::PhysicsWorld(Ambient* ambient)
	: Super(ambient)
	, _fps(DEFAULT_FPS)
{
	time = GetSubSystem<Time>();

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
}


PhysicsWorld::~PhysicsWorld()
{
	SAFE_DELETE(_world);
	SAFE_DELETE(_solver);
	SAFE_DELETE(_broadphase);
	SAFE_DELETE(_collisionDispatcher);
	SAFE_DELETE(_collisionConfiguration);
	if (!PhysicsWorld::config.collisionConfig)
		delete _collisionConfiguration;
}

void PhysicsWorld::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<PhysicsWorld>(SCENE_ATTACH);
}

void PhysicsWorld::Update()
{
	float timeStep = 0.1f;
	SetFPS(1/time->GetDeltaTime());
	float internalTimeStep = 1.0f / _fps;
	int maxSubSteps = (int)(timeStep * _fps) + 1;

	if (maxSubSteps < 0)
	{
		internalTimeStep = timeStep;
		maxSubSteps = 1;
	}
	_world->stepSimulation(timeStep, maxSubSteps, internalTimeStep);
}

void PhysicsWorld::AddRigidBodies(RigidBody* rigidBody)
{
	_rigidBodies.push_back(rigidBody);
}

void PhysicsWorld::RemoveRigidBodies(RigidBody* rigidBody)
{
	for (VECTOR<RigidBody*>::iterator it = _rigidBodies.begin();
		it != _rigidBodies.end();
		it++)
	{
		if (*it == rigidBody)
			_rigidBodies.erase(it);
	}
}

void PhysicsWorld::AddCollider(Collider* collider)
{
	_colliders.push_back(collider);
}

void PhysicsWorld::RemoveCollider(Collider* collider)
{
	for (VECTOR<Collider*>::iterator it = _colliders.begin();
		it != _colliders.end();
		it++)
	{
		if (*it == collider)
			_colliders.erase(it);
	}
}

void PhysicsWorld::AddConstraint(Constraint* constraint)
{
	_constraints.push_back(constraint);
}

void PhysicsWorld::RemoveConstraint(Constraint* constraint)
{
	for (VECTOR<Constraint*>::iterator it = _constraints.begin();
		it != _constraints.end();
		it++)
	{
		if (*it == constraint)
			_constraints.erase(it);
	}
}

void PhysicsWorld::SetFPS(int fps)
{
	_fps = (unsigned)clamp(fps, 1, 1000);
}

void PhysicsWorld::deleteColliders() 
{
	for (int j = 0; j < _colliders.size(); j++)
	{
		btCollisionShape* shape = _colliders[j]->GetShape();
		//_colliders[j]->GetShape() = 0;
		delete shape;
	}
}

}