#include "Collider.h"
#include "Ambient.h"
namespace Auto3D {


Collider::Collider(Ambient* ambient)
	: Super(ambient)
	, _cachedWorldScale(Vector3::one)
{
}


Collider::~Collider()
{
}

void Collider::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<Collider>(SCENE_ATTACH);
}

void Collider::Start()
{
}

}