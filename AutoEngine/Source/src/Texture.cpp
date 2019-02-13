#include "Texture.h"

namespace Auto3D {

Texture::Texture(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, GPUObject(GetSubSystem<Graphics>())
{

}

Texture::~Texture()
{

}

int Texture::GetLevelWidth(unsigned level) const
{
	if (level > _levels)
		return 0;
	return max(_width >> level, 1);
}

int Texture::GetLevelHeight(unsigned level) const
{
	if (level > _levels)
		return 0;
	return max(_height >> level, 1);
}
unsigned Texture::GetDataSize(int width, int height) const
{
	if (IsCompressed())
		return GetRowDataSize(width) * ((height + 3) >> 2u);
	else
		return GetRowDataSize(width) * height;
}

unsigned Texture::GetDataSize(int width, int height, int depth) const
{
	return depth * GetDataSize(width, height);
}

void Texture::SetBackupTexture(SharedPtr<Texture> texture)
{
	_backupTexture = texture;
}
void Texture::SetAddressMode(TextureCoordinate coord, TextureAddressMode mode)
{
	_addressModes[(int)coord] = mode;
	_parametersDirty = true;
}

}