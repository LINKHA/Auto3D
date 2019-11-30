#include "RigidBody2D.h"
#include "PhysicsWorld2D.h"
#include "SpatialNode2D.h"
#include "Scene2D.h"

#include "Core/Modules/ModuleManager.h"
#include "Physics/PhysicsUtils.h"
#include "Physics/Physics.h"


namespace Auto3D {

ARigidBody2D::ARigidBody2D():
	_body(nullptr),
	_useFixtureMass(true)
{
	_physicsWorld2d = GModuleManager::Get().PhysicsModule()->GetActivePhysicsWorld2D();
	// Make sure the massData members are zero-initialized.
	_massData.mass = 0.0f;
	_massData.I = 0.0f;
	_massData.center.SetZero();

}
ARigidBody2D::~ARigidBody2D()
{
	RemoveBodyFromWorld();
}

void ARigidBody2D::RegisterObject()
{
	RegisterFactory<ARigidBody2D>();
}

void ARigidBody2D::SetBodyType(EBodyType2D::Type type)
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

void ARigidBody2D::SetMass(float mass)
{
	mass = Max(mass, 0.0f);
	if (_massData.mass == mass)
		return;

	_massData.mass = mass;

	if (!_useFixtureMass && _body)
		_body->SetMassData(&_massData);
}

void ARigidBody2D::SetInertia(float inertia)
{
	inertia = Max(inertia, 0.0f);
	if (_massData.I == inertia)
		return;

	_massData.I = inertia;

	if (!_useFixtureMass && _body)
		_body->SetMassData(&_massData);

}

void ARigidBody2D::SetMassCenter(const TVector2F& center)
{
	b2Vec2 b2Center = ToB2Vector2(center);
	if (_massData.center == b2Center)
		return;

	_massData.center = b2Center;

	if (!_useFixtureMass && _body)
		_body->SetMassData(&_massData);
}

void ARigidBody2D::ApplyWorldTransform()
{
	if (!_body)
		return;

	ASpatialNode2D* node = dynamic_cast<ASpatialNode2D*>(Parent());

	const b2Transform& transform = _body->GetTransform();
	TVector3F newWorldPosition = node->GetPosition();
	newWorldPosition._x = transform.p.x;
	newWorldPosition._y = transform.p.y;
	FQuaternion newWorldRotation(transform.q.GetAngle() * M_RADTODEG, TVector3F::FORWARD);

	ApplyWorldTransform(newWorldPosition, newWorldRotation);
}

void ARigidBody2D::ApplyWorldTransform(const TVector3F& newWorldPosition, const FQuaternion& newWorldRotation)
{
	ASpatialNode2D* node = dynamic_cast<ASpatialNode2D*>(Parent());

	if (newWorldPosition != node->GetPosition() || newWorldRotation!= node->GetRotation())
	{
		node->SetWorldPosition(newWorldPosition);
		node->SetWorldRotation(newWorldRotation);
	}
}

void ARigidBody2D::ParentCallBack()
{
	_physicsWorld2d = ParentScene2D()->GetPhysicsWorld();

	

	AddBodyToWorld();
}

void ARigidBody2D::AddBodyToWorld()
{
	if (!_physicsWorld2d)
		return;

	_physicsWorld2d->AddRigidBody(this);

	if (_body)
	{
		RemoveBodyFromWorld();
	}
	else
	{
		ASpatialNode2D* parentNode = dynamic_cast<ASpatialNode2D*>(Parent());
		TVector3F nodePosition = parentNode->GetPosition();
		float nodeAngle = parentNode->GetRotation().RollAngle() * M_DEGTORAD;;

		_bodyDef.position.Set(nodePosition._x, nodePosition._y);
		_bodyDef.angle = nodeAngle;

		_body = _physicsWorld2d->GetWorld()->CreateBody(&_bodyDef);
	}
}

void ARigidBody2D::RemoveBodyFromWorld()
{
	if (!_physicsWorld2d)
		return;

	_physicsWorld2d->GetWorld()->DestroyBody(_body);

	_physicsWorld2d->RemoveRigidBody(this);
}

}