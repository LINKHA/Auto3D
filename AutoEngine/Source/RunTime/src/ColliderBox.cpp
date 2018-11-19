#include "ColliderBox.h"
#include "Ambient.h"
#include "PhysicsUtils.h"

namespace Auto3D {


ColliderBox::ColliderBox(Ambient* ambient)
	: Super(ambient)
	, _size(Vector3::one)
{
	_shape = new btBoxShape(ToBtVector3(_size * 0.5f));
	_shape->setLocalScaling(ToBtVector3(_cachedWorldScale));
}


ColliderBox::~ColliderBox()
{
	SAFE_DELETE(_shape);
}

void ColliderBox::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<ColliderBox>(SCENE_ATTACH);
}


}