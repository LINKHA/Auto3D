#include "Resource/Image.h"
#include "RHI/bgfx_utils.h"

namespace Auto3D
{

bool OImage::BeginLoad(const FString& pathName)
{
	_imageContainer = imageLoad(
		pathName.CString()
		, bgfx::TextureFormat::BGRA8
	);
	
	return true;

}

bool OImage::EndLoad()
{
	return true;
}

}