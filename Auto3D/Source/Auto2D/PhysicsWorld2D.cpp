#include "Scene2D.h"
#include "PhysicsWorld2D.h"
#include "RigidBody2D.h"

#include "Physics/PhysicsUtils.h"
#include "Physics/Physics.h"
#include "Core/Modules/ModuleManager.h"


namespace Auto3D {

static const int DEFAULT_FPS = 60;
static const Vector2F DEFAULT_GRAVITY = Vector2F(0.0f, -9.81f);
static const int DEFAULT_VELOCITY_ITERATIONS = 8;
static const int DEFAULT_POSITION_ITERATIONS = 3;

PhysicsWorld2D::PhysicsWorld2D() :
	_fps(DEFAULT_FPS),
	_gravity(DEFAULT_GRAVITY),
	_velocityIterations(DEFAULT_VELOCITY_ITERATIONS),
	_positionIterations(DEFAULT_POSITION_ITERATIONS)
{
	_time = GModuleManager::Get().TimeModule();

	_world = new b2World(ToB2Vector2(_gravity));
	_world->SetContactListener(this);

	// Register to the physics subsystem
	auto physics = GModuleManager::Get().PhysicsModule();
	physics->AddPhysicsWorld2D(this);
	physics->SetActivePhysicsWrold2D(this);
}

PhysicsWorld2D::~PhysicsWorld2D()
{
	auto physics = GModuleManager::Get().PhysicsModule();
	physics->RemovePhysicsWorld2D(this);
	physics->SetActivePhysicsWrold2D(nullptr);

	_world.Reset();
}

void PhysicsWorld2D::RegisterObject()
{
	RegisterFactory<PhysicsWorld2D>();
}

void PhysicsWorld2D::Update()
{

	_world->Step(1.0f/ _fps, _velocityIterations, _positionIterations);
	// Apply world transforms. Unparented transforms first
	for (unsigned i = 0; i < _rigidBodies.Size();)
	{
		if (_rigidBodies[i])
		{
			_rigidBodies[i]->ApplyWorldTransform();
			++i;
		}
		else
		{
			// Erase possible stale weak pointer
			_rigidBodies.Erase(i);
		}
	}

}

void PhysicsWorld2D::SetFPS(int fps)
{
	_fps = (unsigned)Clamp(fps, 1, 1000);
}

void PhysicsWorld2D::AddRigidBody(RigidBody2D* rigidbody)
{
	_rigidBodies.Push(rigidbody);
}

void PhysicsWorld2D::RemoveRigidBody(RigidBody2D* rigidbody)
{
	_rigidBodies.Remove(rigidbody);
}

void PhysicsWorld2D::ParentCallBack()
{
	// Get active scene to set physics world
	ParentScene2D()->SetPhysicsWorld(this);
}

}