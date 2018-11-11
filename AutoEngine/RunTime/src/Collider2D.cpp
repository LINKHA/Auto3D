#include "Collider2D.h"
#include "PhysicsUtils.h"

namespace Auto3D {

Collider2D::Collider2D(Ambient* ambient)
	: Super(ambient)
	, _fixture(nullptr)
{
}


Collider2D::~Collider2D()
{
}

void Collider2D::CreateFixture()
{
	if (_fixture)
		return;

	if (!_fixtureDef.shape)
		return;

	b2Body* body = _rigidBody->GetBody();
	if (!body)
		return;
}

void Collider2D::ReleaseFixture()
{

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
