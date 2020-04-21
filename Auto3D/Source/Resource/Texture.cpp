#include "Resource/Texture.h"
#include "RHI/bgfx_utils.h"

namespace Auto3D
{

bool OTexture::BeginLoad(const FString& pathName)
{
	_textureHandle = loadTexture(pathName.CString());
	if (isValid(_textureHandle))
	{
		return true;
	}
	return false;
}

bool OTexture::EndLoad()
{
	return true;
}

}