#include "PhysicsWorld2D.h"
#include "PhysicsUtils.h"
#include "Ambient.h"
#include "NewDef.h"

namespace Auto3D {

static const Vector2 DEFAULT_GRAVITY(0.0f, -9.81f);
static const int DEFAULT_VELOCITY_ITERATIONS = 8;
static const int DEFAULT_POSITION_ITERATIONS = 3;

PhysicsWorld2D::PhysicsWorld2D(Ambient* ambient)
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
	/*for (unsigned i = 0; i < _rigidBodies.Size(); ++i)
		if (_rigidBodies[i])
			_rigidBodies[i]->ReleaseBody();*/
}

void PhysicsWorld2D::RegisterObject(Ambient* ambient)
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

void PhysicsWorld2D::AddRigidBody(RigidBody2D* rigidBody)
{
	if (!rigidBody)
		return;

	RigidBody2D* rigidBodyPtr(rigidBody);
	// Determine if there is duplication
	if (std::count(_rigidBodys.begin(), _rigidBodys.end(), rigidBodyPtr))
		return;

	_rigidBodys.push_back(rigidBodyPtr);
}

void PhysicsWorld2D::RemoveRigidBody(RigidBody2D* rigidBody)
{
	if (!rigidBody)
		return;
	for(VECTOR<RigidBody2D*>::iterator it = _rigidBodys.begin(); it != _rigidBodys.end(); it++)
	{
		if (*it == rigidBody)
		{
			_rigidBodys.erase(it);
			return;
		}
	}
	
}

}