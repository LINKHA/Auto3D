#pragma once
#include "AutoConfig.h"
#include "Resource/Resource.h"
#include <bgfx/bgfx.h>

namespace Auto3D
{

class AUTO_API OShader : public OResource
{
	DECLARE_O_CLASS(OShader, OResource)
public:
	OShader() {}
	virtual ~OShader() {}

	/// Load the resource data from a stream. May be executed outside the main thread, should not access GPU resources. Return true on success.
	virtual bool BeginLoad(const FString& pathName) override;
	/// Finish resource loading if necessary. Always called from the main thread, so GPU resources can be accessed here. Return true on success.
	virtual bool EndLoad()override;

	bgfx::ShaderHandle GetShaderHandle() { return _shaderHandle; }
private:
	bgfx::ShaderHandle _shaderHandle;
};

}