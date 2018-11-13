#include "ColliderBox2D.h"
#include "Ambient.h"
namespace Auto3D {

ColliderBox2D::ColliderBox2D(Ambient* ambient)
	:Super(ambient)
{
}


ColliderBox2D::~ColliderBox2D()
{
}

void ColliderBox2D::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<ColliderBox2D>(SCENE_ATTACH);
}

}