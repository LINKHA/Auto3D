#include "Collider2D.h"
#include "Ambient.h"
#include "RigidBody2D.h"
#include "NewDef.h"

namespace Auto3D {

Collider2D::Collider2D(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _fixture(nullptr)
	, _cachedWorldScale(Vector3::one)
{
}


Collider2D::~Collider2D()
{
}

void Collider2D::RegisterObject(SharedPtr<Ambient> ambient)
{
	ambient->RegisterFactory<Collider2D>(SCENE_ATTACH);
}
void Collider2D::Start()
{
	CreateFixture();
}
void Collider2D::CreateFixture()
{
	if (_fixture || !_fixtureDef.shape)
		return;

	if (!_rigidBody)
	{
		// RigidBody2D can be created after CollisionShape2D
		_rigidBody = GetNode().GetComponent<RigidBody2D>(); 
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
	if (!_fixture || !_rigidBody)
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

void Collider2D::SetTrigger(bool trigger)
{
	if (_fixtureDef.isSensor == trigger)
		return;

	_fixtureDef.isSensor = trigger;

	if (_fixture)
		_fixture->SetSensor(trigger);
}

void Collider2D::SetCategoryBits(int categoryBits)
{
	if (_fixtureDef.filter.categoryBits == categoryBits)
		return;

	_fixtureDef.filter.categoryBits = (uint16)categoryBits;

	if (_fixture)
		_fixture->SetFilterData(_fixtureDef.filter);
}

void Collider2D::SetMaskBits(int maskBits)
{
	if (_fixtureDef.filter.maskBits == maskBits)
		return;

	_fixtureDef.filter.maskBits = (uint16)maskBits;

	if (_fixture)
		_fixture->SetFilterData(_fixtureDef.filter);
}

void Collider2D::SetGroupIndex(int groupIndex)
{
	if (_fixtureDef.filter.groupIndex == groupIndex)
		return;

	_fixtureDef.filter.groupIndex = (int16)groupIndex;

	if (_fixture)
		_fixture->SetFilterData(_fixtureDef.filter);
}

void Collider2D::SetDensity(float density)
{
	if (_fixtureDef.density == density)
		return;

	_fixtureDef.density = density;

	if (_fixture)
	{
		// This will not automatically adjust the mass of the body
		_fixture->SetDensity(density);

		if (_rigidBody->GetUseFixtureMass())
			_rigidBody->GetBody()->ResetMassData();
	}
}

void Collider2D::SetFriction(float friction) 
{
	if (_fixtureDef.friction == friction)
		return;

	_fixtureDef.friction = friction;

	if (_fixture)
	{
		// This will not change the friction of existing contacts
		_fixture->SetFriction(friction);

		b2ContactEdge* contractEdge = _rigidBody->GetBody()->GetContactList();
		while (contractEdge)
		{
			b2Contact* contact = contractEdge->contact;
			if (contact->GetFixtureA() == _fixture || contact->GetFixtureB() == _fixture)
				contractEdge->contact->ResetFriction();

			contractEdge = contractEdge->next;
		}
	}
}

void Collider2D::SetRestitution(float restitution)
{
	if (_fixtureDef.restitution == restitution)
		return;

	_fixtureDef.restitution = restitution;

	if (_fixture)
	{
		// This will not change the restitution of existing contacts
		_fixture->SetRestitution(restitution);

		b2ContactEdge* contractEdge = _rigidBody->GetBody()->GetContactList();
		while (contractEdge)
		{
			b2Contact* contact = contractEdge->contact;
			if (contact->GetFixtureA() == _fixture || contact->GetFixtureB() == _fixture)
				contractEdge->contact->ResetRestitution();

			contractEdge = contractEdge->next;
		}
	}
}


float Collider2D::GetMass() const
{
	if (!_fixture)
		return 0.0f;

	b2MassData massData;
	_fixture->GetMassData(&massData);

	return massData.mass;
}

float Collider2D::GetInertia() const
{
	if (!_fixture)
		return 0.0f;

	b2MassData massData;
	_fixture->GetMassData(&massData);

	return massData.I;
}

Vector2 Collider2D::GetMassCenter() const
{
	if (!_fixture)
		return Vector2::zero;

	b2MassData massData;
	_fixture->GetMassData(&massData);

	return ToVector2(massData.center);
}


}
