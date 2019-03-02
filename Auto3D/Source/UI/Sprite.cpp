#include "Sprite.h"
#include "../Graphics/Texture.h"
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

void Sprite::SetTexture(Texture* texture)
{
	_texture = texture;
}

}