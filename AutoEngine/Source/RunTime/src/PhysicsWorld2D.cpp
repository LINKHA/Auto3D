#include "PhysicsWorld2D.h"
#include "PhysicsUtils.h"
#include "RigidBody2D.h"
#include "Ambient.h"
#include "NewDef.h"

namespace Auto3D {

static const Vector2 DEFAULT_GRAVITY(0.0f, -9.81f);
static const int DEFAULT_VELOCITY_ITERATIONS = 8;
static const int DEFAULT_POSITION_ITERATIONS = 3;

PhysicsWorld2D::PhysicsWorld2D(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _gravity(DEFAULT_GRAVITY)
	, _velocityIter(DEFAULT_VELOCITY_ITERATIONS)
	, _positionIter(DEFAULT_POSITION_ITERATIONS)
{
	// Create physics 2D world
	_world = new b2World(ToB2Vec2(_gravity));
	// Set contact listener
	//_world->SetContactListener(this);
	// Set debug draw
	//_world->SetDebugDraw(this);
}

PhysicsWorld2D::~PhysicsWorld2D()
{
	SafeDelete(_world);
	for (unsigned i = 0; i < _rigidBodys.size(); ++i)
		if (_rigidBodys[i])
			_rigidBodys[i]->ReleaseBody();
}

void PhysicsWorld2D::RegisterObject(SharedPtr<Ambient> ambient)
{
	ambient->RegisterFactory<PhysicsWorld2D>(SCENE_ATTACH);
}

void PhysicsWorld2D::Update()
{
	float timeStep = 1.0f / 10.0f;

	_isPhysicsStepping = true;
	_world->Step(timeStep, _velocityIter, _positionIter);

	_isPhysicsStepping = false;
}

void PhysicsWorld2D::AddRigidBody(SharedPtr<RigidBody2D> rigidBody)
{
	if (!rigidBody)
		return;

	// Determine if there is duplication
	if (std::count(_rigidBodys.begin(), _rigidBodys.end(), rigidBody))
		return;

	_rigidBodys.push_back(rigidBody);
}

void PhysicsWorld2D::RemoveRigidBody(SharedPtr<RigidBody2D> rigidBody)
{
	if (!rigidBody)
		return;
	VectorFindEarse(_rigidBodys,rigidBody);
}

}