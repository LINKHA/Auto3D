#include "RigidBody2D.h"
#include "PhysicsWorld2D.h"
#include "SpatialNode2D.h"
#include "Scene2D.h"

#include "../Engine/ModuleManager.h"
#include "../Physics/PhysicsUtils.h"
#include "../Physics/Physics.h"


namespace Auto3D {

RigidBody2D::RigidBody2D():
	_body(nullptr),
	_useFixtureMass(true)
{
	_physicsWorld2d = ModuleManager::Get().PhysicsModule()->GetActivePhysicsWorld2D();


}
RigidBody2D::~RigidBody2D()
{

}

void RigidBody2D::RegisterObject()
{
	RegisterFactory<RigidBody2D>();
}

void RigidBody2D::SetBodyType(BodyType2D::Type type)
{
	auto bodyType = (b2BodyType)type;
	if (_body)
	{
		_body->SetType(bodyType);
		// Mass data was reset to keep it legal (e.g. static body should have mass 0.)
		// If not using fixture mass, reassign our mass data now
		if (!_useFixtureMass)
			_body->SetMassData(&_massData);
	}
	else
	{
		if (_bodyDef.type == bodyType)
			return;

		_bodyDef.type = bodyType;
	}
}

void RigidBody2D::SetMass(float mass)
{
	mass = Max(mass, 0.0f);
	if (_massData.mass == mass)
		return;

	_massData.mass = mass;

	if (!_useFixtureMass && _body)
		_body->SetMassData(&_massData);
}

void RigidBody2D::SetInertia(float inertia)
{
	inertia = Max(inertia, 0.0f);
	if (_massData.I == inertia)
		return;

	_massData.I = inertia;

	if (!_useFixtureMass && _body)
		_body->SetMassData(&_massData);

}

void RigidBody2D::SetMassCenter(const Vector2F& center)
{
	b2Vec2 b2Center = ToB2Vector2(center);
	if (_massData.center == b2Center)
		return;

	_massData.center = b2Center;

	if (!_useFixtureMass && _body)
		_body->SetMassData(&_massData);
}

void RigidBody2D::ApplyWorldTransform()
{
	if (!_body)
		return;

	SpatialNode2D* node = dynamic_cast<SpatialNode2D*>(Parent());

	const b2Transform& transform = _body->GetTransform();
	Vector3F newWorldPosition = node->GetPosition();
	newWorldPosition._x = transform.p.x;
	newWorldPosition._y = transform.p.y;
	Quaternion newWorldRotation(transform.q.GetAngle() * M_RADTODEG, Vector3F::FORWARD);

	ApplyWorldTransform(newWorldPosition, newWorldRotation);
}

void RigidBody2D::ApplyWorldTransform(const Vector3F& newWorldPosition, const Quaternion& newWorldRotation)
{
	SpatialNode2D* node = dynamic_cast<SpatialNode2D*>(Parent());

	if (newWorldPosition != node->GetPosition() || newWorldRotation!= node->GetRotation())
	{
		node->SetWorldPosition(newWorldPosition);
		node->SetWorldRotation(newWorldRotation);
	}
}

void RigidBody2D::ParentCallBack()
{
	_physicsWorld2d = ParentScene2D()->GetPhysicsWorld();

	_physicsWorld2d->AddRigidBody(this);

	AddBodyToWorld();
}

void RigidBody2D::AddBodyToWorld()
{
	if (!_physicsWorld2d)
		return;

	if (_body)
	{
		RemoveBodyFromWorld();
	}
	else
	{
		SpatialNode2D* parentNode = dynamic_cast<SpatialNode2D*>(Parent());
		Vector3F nodePosition = parentNode->GetPosition();
		float nodeAngle = parentNode->GetRotation().RollAngle() * M_DEGTORAD;;

		_bodyDef.position.Set(nodePosition._x, nodePosition._y);
		_bodyDef.angle = nodeAngle;

		_body = _physicsWorld2d->GetWorld()->CreateBody(&_bodyDef);
	}
}

void RigidBody2D::RemoveBodyFromWorld()
{

}

}