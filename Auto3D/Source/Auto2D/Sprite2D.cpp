#include "Sprite2D.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

ASprite2D::ASprite2D() = default;

ASprite2D::~ASprite2D() = default;

void ASprite2D::RegisterObject()
{
	RegisterFactory<ASprite2D>();
}

}