#include "RigidBody2D.h"


namespace Auto3D {

RigidBody2D::RigidBody2D(Ambient* ambient)
	: Super(ambient)
	, _useFixtureMass(true)
	, _body(nullptr)
{
	_massData.mass = 0.0f;
	_massData.I = 0.0f;
	_massData.center.SetZero();
}


RigidBody2D::~RigidBody2D()
{
}

void OnSceneSet(PhysicsWorld2D* physicsWorld)
{

}

}
