#pragma once
#include "AutoConfig.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/RendererDef.h"

#include <bgfx/bgfx.h>
#include <bx/math.h>

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
		_pbrMesh.AttachShader("vs_pbr_mesh", "fs_pbr_mesh");

		us_brdfLUT = bgfx::createUniform("s_brdfLUT", bgfx::UniformType::Sampler);
		us_albedoMap = bgfx::createUniform("s_albedoMap", bgfx::UniformType::Sampler);
		us_normalMap = bgfx::createUniform("s_normalMap", bgfx::UniformType::Sampler);
		us_metallicMap = bgfx::createUniform("s_metallicMap", bgfx::UniformType::Sampler);
		us_roughnessMap = bgfx::createUniform("s_roughnessMap", bgfx::UniformType::Sampler);
		us_aoMap = bgfx::createUniform("s_aoMap", bgfx::UniformType::Sampler);


		_brdfLUTFrame = bgfx::createFrameBuffer(512, 512, bgfx::TextureFormat::BGRA8);

	}
	void Update()
	{
		const bgfx::Caps* m_caps = bgfx::getCaps();
		float proj[16];
		bx::mtxOrtho(proj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0f, m_caps->homogeneousDepth);

		bgfx::setViewTransform(RENDERVIEW_BRDF_LUT_ID, NULL, proj);
		bgfx::setViewRect(RENDERVIEW_BRDF_LUT_ID, 0, 0, 512, 512);
		bgfx::setViewFrameBuffer(RENDERVIEW_BRDF_LUT_ID, _brdfLUTFrame);

		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		screenSpaceQuad(512, 512, true);
		bgfx::submit(RENDERVIEW_BRDF_LUT_ID, _brdfLUT.GetProgram());
	}
	bgfx::UniformHandle us_brdfLUT;
	bgfx::UniformHandle us_albedoMap;
	bgfx::UniformHandle us_normalMap;
	bgfx::UniformHandle us_metallicMap;
	bgfx::UniformHandle us_roughnessMap;
	bgfx::UniformHandle us_aoMap;

	FShaderProgram _brdfLUT;
	FShaderProgram _pbrMesh;

	bgfx::FrameBufferHandle _brdfLUTFrame;
};

}