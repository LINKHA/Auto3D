#include "Sprite.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

Sprite::Sprite() = default;

Sprite::~Sprite() = default;

void Sprite::RegisterObject()
{
	RegisterFactory<Sprite>();
}

}