#include "Sprite2D.h"
#include "Ambient.h"

namespace Auto3D {

Sprite2D::Sprite2D(Ambient* ambient)
	:Super(ambient)
{
}

Sprite2D::~Sprite2D()
{
}

void Sprite2D::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<Sprite2D>(RESOURCE_ATTACH);
}

}