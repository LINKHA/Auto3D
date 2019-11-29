#include "PhysicsWorld.h"
#include "PhysicsUtils.h"
#include "RegisteredBox/RegisteredBox.h"
#include "Scene/Scene.h"
#include "Physics/Physics.h"
#include "Core/Modules/ModuleManager.h"

namespace Auto3D {

const char* PHYSICS_CATEGORY = "Physics";
extern const char* SUBSYSTEM_CATEGORY;

static const int MAX_SOLVER_ITERATIONS = 256;
static const Vector3F DEFAULT_GRAVITY = Vector3F(0.0f, -9.81f, 0.0f);
static const int DEFAULT_FPS = 60;
static const float DEFAULT_MAX_NETWORK_ANGULAR_VELOCITY = 100.0f;

PhysicsWorldConfig PhysicsWorld::config;

PhysicsWorld::PhysicsWorld():
	_fps(DEFAULT_FPS),
	_maxSubSteps(0)
{
	_time = GModuleManager::Get().TimeModule();

	if (PhysicsWorld::config.collisionConfig)
		_collisionConfiguration = PhysicsWorld::config.collisionConfig;
	else
		_collisionConfiguration = new btDefaultCollisionConfiguration();

	_collisionDispatcher = new btCollisionDispatcher(_collisionConfiguration.Get());
	_broadphase = new btDbvtBroadphase();
	_solver = new btSequentialImpulseConstraintSolver();
	_world = new btDiscreteDynamicsWorld(_collisionDispatcher.Get(), _broadphase.Get(), _solver.Get(), _collisionConfiguration.Get());

	_world->setGravity(ToBtVector3(DEFAULT_GRAVITY));

	_world->setSynchronizeAllMotionStates(true);

	// Register to the physics subsystem
	auto physics = GModuleManager::Get().PhysicsModule();
	physics->AddPhysicsWorld(this);
	physics->SetActivePhysicsWorld(this);
}

PhysicsWorld::~PhysicsWorld()
{
	auto physics = GModuleManager::Get().PhysicsModule();
	physics->RemovePhysicsWorld(this);
	physics->SetActivePhysicsWorld(nullptr);

	_world.Reset();
	_solver.Reset();
	_broadphase.Reset();
	_collisionDispatcher.Reset();
	if (!PhysicsWorld::config.collisionConfig)
		_collisionConfiguration.Reset();
}

void PhysicsWorld::RegisterObject()
{
	RegisterFactory<PhysicsWorld>();
}

void PhysicsWorld::Update()
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

void PhysicsWorld::SetFPS(int fps)
{
	_fps = (unsigned)Clamp(fps, 1, 1000);
}

void PhysicsWorld::AddRigidBody(RigidBody* rigidbody)
{
	_rigidBody.Push(rigidbody);
}

void PhysicsWorld::RemoveRigidBody(RigidBody* rigidbody)
{
	_rigidBody.Remove(rigidbody);
}


void PhysicsWorld::ClearColliders()
{
	_rigidBody.Clear();
}

void PhysicsWorld::ParentCallBack()
{
	// Get active scene to set physics world
	ParentScene()->SetPhysicsWorld(this);
}

}