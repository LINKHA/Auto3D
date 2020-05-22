#pragma once
#include "Resource/Resource.h"
#include "Container/Ptr.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{
//	Cubemap layers numbers
//		 ---
//		| 3	|
//	 --- --- --- ---	
//	| 1 | 4 | 0 | 5 |
//	 --- --- --- ---
//		| 2	|
//		 ---
class AUTO_API OTexture : public OResource
{
	DECLARE_O_CLASS(OTexture, OResource)
public:
	OTexture() {}
	virtual ~OTexture() {}

	/// Load the resource data from a stream. May be executed outside the main thread, should not access GPU resources. Return true on success.
	virtual bool BeginLoad(const FString& pathName) override;
	/// Finish resource loading if necessary. Always called from the main thread, so GPU resources can be accessed here. Return true on success.
	virtual bool EndLoad()override;

	bgfx::TextureHandle GetTextureHandle() { return _textureHandle; }
private:
	bgfx::TextureHandle _textureHandle;
};

}