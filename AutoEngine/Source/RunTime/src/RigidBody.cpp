#include "RigidBody.h"
#include "Ambient.h"
#include "PhysicsWorld2D.h"
#include "PhysicsUtils.h"
#include "Collider2D.h"

namespace Auto3D {

RigidBody::RigidBody(Ambient* ambient)
	:Super(ambient)
{
}


RigidBody::~RigidBody()
{
}

void RigidBody::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<RigidBody>(SCENE_ATTACH);
}

void RigidBody::AddBodyToWorld()
{
}

}