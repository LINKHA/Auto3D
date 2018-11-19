#include "RigidBody2D.h"
#include "Ambient.h"
#include "PhysicsWorld2D.h"
#include "PhysicsUtils.h"
#include "Collider2D.h"

namespace Auto3D {

static const char* bodyTypeNames[] =
{
	"Static",
	"Kinematic",
	"Dynamic",
	nullptr
};

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
	if (_physicsWorld)
	{
		ReleaseBody();
		_physicsWorld->RemoveRigidBody(this);
	}
}

void RigidBody2D::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<RigidBody2D>(SCENE_ATTACH);
}

void RigidBody2D::Start()
{
	_physicsWorld = GetCurrentSceneNode()->GetPhysicsWorld2D();
	CreateBody();
}

void RigidBody2D::Update()
{
	b2Vec2 tmpPos = _body->GetPosition();
	GetGameObjectPtr()->SetPosition(tmpPos.x, tmpPos.y);
}

void RigidBody2D::CreateBody()
{
	PhysicsWorld2D* physicsWorld = GetCurrentSceneNode()->GetPhysicsWorld2D();
	if (_body)
		return;

	if (!physicsWorld || !physicsWorld->GetWorld())
		return;

	_bodyDef.position = ToB2Vec2(GetGameObjectPtr()->GetPosition());
#pragma warning
	//_bodyDef.angle = node_->GetWorldRotation().RollAngle() * MATH_DEGTORAD;

	_body = physicsWorld->GetWorld()->CreateBody(&_bodyDef);
	_body->SetUserData(this);
}

void RigidBody2D::ReleaseBody()
{
	PhysicsWorld2D* physicsWorld = GetCurrentSceneNode()->GetPhysicsWorld2D();
	if (_body)
		return;

	if (!physicsWorld || !physicsWorld->GetWorld())
		return;
}

void RigidBody2D::SetBodyType(BodyType2D type)
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

}
