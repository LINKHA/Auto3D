#include "Sprite.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

Sprite::Sprite()
{

}

Sprite::~Sprite()
{

}

void Sprite::RegisterObject()
{
	RegisterFactory<Sprite>();
}



}