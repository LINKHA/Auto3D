#include "Sprite2D.h"
#include "Ambient.h"
#include "NewDef.h"

namespace Auto3D {

Sprite::Sprite(Ambient* ambient)
	:Super(ambient)
{
}

Sprite::~Sprite()
{
}

void Sprite::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<Sprite>(RESOURCE_ATTACH);
}

}