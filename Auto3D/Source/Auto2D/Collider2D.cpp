#include "Collider2D.h"
#include "PhysicsWorld2D.h"
#include "Scene2D.h"
#include "RigidBody2D.h"

namespace Auto3D {

ACollider2D::ACollider2D():
	_fixture(nullptr)
{
	SetDensity(0.5f);
}

ACollider2D::~ACollider2D()
{
	ReleaseFixture();
}

void ACollider2D::RegisterObject()
{
	RegisterFactory<ACollider2D>();
}

void ACollider2D::SetDensity(float density)
{
	if (_fixtureDef.density == density)
		return;

	_fixtureDef.density = density;

	ReleaseFixture();
	CreateFixture();
}

void ACollider2D::SetFriction(float friction)
{
	if (_fixtureDef.friction == friction)
		return;

	_fixtureDef.friction = friction;

	ReleaseFixture();
	CreateFixture();
}

void ACollider2D::SetRestitution(float restitution)
{
	if (_fixtureDef.restitution == restitution)
		return;

	_fixtureDef.restitution = restitution;

	ReleaseFixture();
	CreateFixture();
}

void ACollider2D::CreateFixture()
{
	if (_fixture)
		return;

	if (!_fixtureDef.shape)
		return;

	if (!_rigidBody)
	{
		_rigidBody = Parent()->FindChild<ARigidBody2D>();
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

void ACollider2D::ReleaseFixture()
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

void ACollider2D::ParentCallBack()
{
	_physicsWorld = ParentScene2D()->GetPhysicsWorld();
}

}