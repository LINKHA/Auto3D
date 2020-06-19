#pragma once
#include "Resource/Resource.h"
#include "Container/Ptr.h"

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

namespace Auto3D
{
	
class AUTO_API OImage : public OResource
{
	DECLARE_O_CLASS(OImage, OResource)
public:
	OImage() {}
	virtual ~OImage() {}

	/// Load the resource data from a stream. May be executed outside the main thread, should not access GPU resources. Return true on success.
	virtual bool BeginLoad(const FString& pathName) override;
	/// Finish resource loading if necessary. Always called from the main thread, so GPU resources can be accessed here. Return true on success.
	virtual bool EndLoad()override;


	bimg::ImageContainer* GetImageContainer() { return _imageContainer; }
private:
	bimg::ImageContainer* _imageContainer;
};

}