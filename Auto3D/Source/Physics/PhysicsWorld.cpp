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
	_fps(DEFAULT_FPS),
	_maxSubSteps(0)
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

	_world->setSynchronizeAllMotionStates(true);
}


PhysicsWorld::~PhysicsWorld()
{
	ClearColliders();

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

	//print positions of all objects
	for (int j = _world->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* obj = _world->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}

		printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	}
}

void PhysicsWorld::AddCollider(Collider* collider)
{
	_colliders.Push(collider);
}

void PhysicsWorld::RemoveCollider(Collider* collider)
{
	_colliders.Remove(collider);
}

void PhysicsWorld::SetFPS(int fps)
{
	_fps = (unsigned)Clamp(fps, 1, 1000);
}

void PhysicsWorld::ClearColliders()
{
	_colliders.Clear();
}

void PhysicsWorld::ParentCallBack()
{
	// Get active scene to set physics world
	ParentScene()->SetPhysicsWorld(this);
}

}