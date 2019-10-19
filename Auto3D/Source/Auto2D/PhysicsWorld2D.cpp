#include "Scene2D.h"
#include "PhysicsWorld2D.h"
#include "../Physics/PhysicsUtils.h"
#include "../Physics/Physics.h"
#include "../Engine/ModuleManager.h"


namespace Auto3D {

static const int DEFAULT_FPS = 60;
static const Vector2F DEFAULT_GRAVITY = Vector2F(0.0f, -9.81f);

PhysicsWorld2D::PhysicsWorld2D() :
	_fps(DEFAULT_FPS)
{
	_time = ModuleManager::Get().TimeModule();

	_world = new b2World(ToB2Vector2(DEFAULT_GRAVITY));

	// Register to the physics subsystem
	auto physics = ModuleManager::Get().PhysicsModule();
	physics->AddPhysicsWorld2D(this);
	physics->SetActivePhysicsWrold2D(this);
}

PhysicsWorld2D::~PhysicsWorld2D()
{
	auto physics = ModuleManager::Get().PhysicsModule();
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
	int velocityIterations = 6;
	int positionIterations = 2;

	_world->Step(_fps, velocityIterations, positionIterations);
}

void PhysicsWorld2D::SetFPS(int fps)
{
	_fps = (unsigned)Clamp(fps, 1, 1000);
}

void PhysicsWorld2D::AddRigidBody(RigidBody2D* rigidbody)
{
	_rigidBody.Push(rigidbody);
}

void PhysicsWorld2D::RemoveRigidBody(RigidBody2D* rigidbody)
{
	_rigidBody.Remove(rigidbody);
}

void PhysicsWorld2D::ParentCallBack()
{
	// Get active scene to set physics world
	ParentScene2D()->SetPhysicsWorld(this);
}

}