#include "Collider2D.h"
#include "PhysicsWorld2D.h"
#include "Scene2D.h"
#include "RigidBody2D.h"

namespace Auto3D {

Collider2D::Collider2D():
	_fixture(nullptr)
{
	SetDensity(0.5f);
}

Collider2D::~Collider2D()
{
	ReleaseFixture();
}

void Collider2D::RegisterObject()
{
	RegisterFactory<Collider2D>();
}

void Collider2D::SetDensity(float density)
{
	if (_fixtureDef.density == density)
		return;

	_fixtureDef.density = density;

	ReleaseFixture();
	CreateFixture();
}

void Collider2D::SetFriction(float friction)
{
	if (_fixtureDef.friction == friction)
		return;

	_fixtureDef.friction = friction;

	ReleaseFixture();
	CreateFixture();
}

void Collider2D::SetRestitution(float restitution)
{
	if (_fixtureDef.restitution == restitution)
		return;

	_fixtureDef.restitution = restitution;

	ReleaseFixture();
	CreateFixture();
}

void Collider2D::CreateFixture()
{
	if (_fixture)
		return;

	if (!_fixtureDef.shape)
		return;

	if (!_rigidBody)
	{
		_rigidBody = Parent()->FindChild<RigidBody2D>();
		if (!_rigidBody)
			return;
	}

	b2Body* body = _rigidBody->GetBody();

	if (!body)
		return;

	// Chain shape must have atleast two vertices before creating fixture
	if (_fixtureDef.shape->m_type != b2Shape::e_chain || static_cast<const b2ChainShape*>(_fixtureDef.shape)->m_count >= 2)
	{
		b2MassData massData;
		body->GetMassData(&massData);

		_fixture = body->CreateFixture(&_fixtureDef);

		// Workaround for resetting mass in CreateFixture().
		if (!_rigidBody->GetUseFixtureMass()) 
			body->SetMassData(&massData);
		_fixture->SetUserData(this);
	}
}

void Collider2D::ReleaseFixture()
{
	if (!_fixture)
		return;

	if (!_rigidBody)
		return;

	b2Body* body = _rigidBody->GetBody();
	if (!body)
		return;

	b2MassData massData;
	body->GetMassData(&massData);
	body->DestroyFixture(_fixture);
	if (!_rigidBody->GetUseFixtureMass()) // Workaround for resetting mass in DestroyFixture().
		body->SetMassData(&massData);
	_fixture = nullptr;
}

void Collider2D::ParentCallBack()
{
	_physicsWorld = ParentScene2D()->GetPhysicsWorld();
}

}