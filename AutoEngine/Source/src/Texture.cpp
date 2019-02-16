#include "Texture.h"

namespace Auto3D {

Texture::Texture(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, GPUObject(GetSubSystem<Graphics>())
{

}

Texture::~Texture() = default;

void Texture::SetNumLevels(unsigned levels)
{
	if (_usage > TextureUsage::RenderTarget)
		_requestedLevels = 1;
	else
		_requestedLevels = levels;
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

void Texture::SetLevelsDirty()
{
	if (_usage == TextureUsage::RenderTarget && _levels > 1)
		_levelsDirty = true;
}

unsigned Texture::CheckMaxLevels(int width, int height, unsigned requestedLevels)
{
	unsigned maxLevels = 1;
	while (width > 1 || height > 1)
	{
		++maxLevels;
		width = width > 1 ? (width >> 1u) : 1;
		height = height > 1 ? (height >> 1u) : 1;
	}

	if (!requestedLevels || maxLevels < requestedLevels)
		return maxLevels;
	else
		return requestedLevels;
}

unsigned Texture::CheckMaxLevels(int width, int height, int depth, unsigned requestedLevels)
{
	unsigned maxLevels = 1;
	while (width > 1 || height > 1 || depth > 1)
	{
		++maxLevels;
		width = width > 1 ? (width >> 1u) : 1;
		height = height > 1 ? (height >> 1u) : 1;
		depth = depth > 1 ? (depth >> 1u) : 1;
	}

	if (!requestedLevels || maxLevels < requestedLevels)
		return maxLevels;
	else
		return requestedLevels;
}

}