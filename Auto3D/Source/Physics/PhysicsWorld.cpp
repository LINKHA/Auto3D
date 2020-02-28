#include "Physics/PhysicsWorld.h"
#include "Physics/PhysicsUtils.h"
#include "Physics/Physics.h"

namespace Auto3D {

const char* PHYSICS_CATEGORY = "Physics";
extern const char* SUBSYSTEM_CATEGORY;

static const int MAX_SOLVER_ITERATIONS = 256;
static const TVector3F DEFAULT_GRAVITY = TVector3F(0.0f, -9.81f, 0.0f);
static const int DEFAULT_FPS = 60;
static const float DEFAULT_MAX_NETWORK_ANGULAR_VELOCITY = 100.0f;

FPhysicsWorldConfig FPhysicsWorld::config;

FPhysicsWorld::FPhysicsWorld():
	_fps(DEFAULT_FPS),
	_maxSubSteps(0)
{
	if (FPhysicsWorld::config.collisionConfig)
		_collisionConfiguration = FPhysicsWorld::config.collisionConfig;
	else
		_collisionConfiguration = new btDefaultCollisionConfiguration();

	_collisionDispatcher = new btCollisionDispatcher(_collisionConfiguration.Get());
	_broadphase = new btDbvtBroadphase();
	_solver = new btSequentialImpulseConstraintSolver();
	_world = new btDiscreteDynamicsWorld(_collisionDispatcher.Get(), _broadphase.Get(), _solver.Get(), _collisionConfiguration.Get());

	_world->setGravity(ToBtVector3(DEFAULT_GRAVITY));

	_world->setSynchronizeAllMotionStates(true);

	// Register to the physics subsystem
	GPhysicsModule& physics = GPhysicsModule::Get();
	physics.AddPhysicsWorld(this);
	physics.SetActivePhysicsWorld(this);
}

FPhysicsWorld::~FPhysicsWorld()
{
	GPhysicsModule& physics = GPhysicsModule::Get();
	physics.RemovePhysicsWorld(this);
	physics.SetActivePhysicsWorld(nullptr);

	_world.Reset();
	_solver.Reset();
	_broadphase.Reset();
	_collisionDispatcher.Reset();
	if (!FPhysicsWorld::config.collisionConfig)
		_collisionConfiguration.Reset();
}

void FPhysicsWorld::Update()
{
	/*float timeStep = 0.1f;
	float internalTimeStep = 1.0f / _fps;
	int maxSubSteps = (int)(timeStep * _fps) + 1;

	if (_maxSubSteps < 0)
	{
		internalTimeStep = timeStep;
		maxSubSteps = 1;
	}
	else if (_maxSubSteps > 0)
		maxSubSteps = Min(maxSubSteps, _maxSubSteps);

	_world->stepSimulation(timeStep, maxSubSteps, internalTimeStep);*/

	_world->stepSimulation(1.f / 60.f, 10);

}

void FPhysicsWorld::SetFPS(int fps)
{
	_fps = (unsigned)Clamp(fps, 1, 1000);
}

void FPhysicsWorld::AddRigidBody(ARigidBodyComponent* rigidbody)
{
	_rigidBody.Push(rigidbody);
}

void FPhysicsWorld::RemoveRigidBody(ARigidBodyComponent* rigidbody)
{
	_rigidBody.Remove(rigidbody);
}


void FPhysicsWorld::ClearColliders()
{
	_rigidBody.Clear();
}

}