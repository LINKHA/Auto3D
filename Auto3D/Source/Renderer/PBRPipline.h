#pragma once
#include "AutoConfig.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/RendererDef.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{

class AUTO_API FPBRPipline
{
public:
	FPBRPipline();
	~FPBRPipline();

	void Init()
	{
		_brdfLUT.AttachShader("vs_brdf_lut", "fs_brdf_lut");
		
		s_brdfLUT = bgfx::createUniform("s_brdfLUT", bgfx::UniformType::Sampler);
		_brdfLUTFrame = bgfx::createFrameBuffer(512, 512, bgfx::TextureFormat::BGRA8);

	}

	void Update()
	{
		const bgfx::Caps* m_caps = bgfx::getCaps();

		bgfx::setViewRect(RENDERVIEW_BRDF_LUT_ID, 0, 0, 512, 512);
		bgfx::setViewFrameBuffer(RENDERVIEW_BRDF_LUT_ID, _brdfLUTFrame);

		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		screenSpaceQuad(512, 512, m_caps->originBottomLeft);
		bgfx::submit(RENDERVIEW_BRDF_LUT_ID, _brdfLUT.GetProgram());

		//bgfx::setTexture(2, s_brdfLUT, bgfx::getTexture(_brdfLUTFrame));
	}

	bgfx::UniformHandle s_brdfLUT;

	FShaderProgram _brdfLUT;

	bgfx::FrameBufferHandle _brdfLUTFrame;
};

}