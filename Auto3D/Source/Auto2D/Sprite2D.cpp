#include "Sprite2D.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

Sprite2D::Sprite2D() = default;

Sprite2D::~Sprite2D() = default;

void Sprite2D::RegisterObject()
{
	RegisterFactory<Sprite2D>();
}

}