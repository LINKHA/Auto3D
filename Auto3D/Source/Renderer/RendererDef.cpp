#include "Renderer//RendererDef.h"
#include <bgfx/bgfx.h>

namespace Auto3D
{

FRenderState FRenderState::_renderState[FRenderState::Count] =
{
	{ // Default
	0
	| BGFX_STATE_WRITE_RGB
	| BGFX_STATE_WRITE_A
	| BGFX_STATE_DEPTH_TEST_LEQUAL
	| BGFX_STATE_WRITE_Z
	| BGFX_STATE_CULL_CCW
	| BGFX_STATE_MSAA
	, UINT32_MAX
	, BGFX_STENCIL_NONE
	, BGFX_STENCIL_NONE
},
{ // ShadowMap_PackDepth
	0
	| BGFX_STATE_WRITE_RGB
	| BGFX_STATE_WRITE_A
	| BGFX_STATE_WRITE_Z
	| BGFX_STATE_DEPTH_TEST_LEQUAL
	| BGFX_STATE_CULL_CCW
	| BGFX_STATE_MSAA
	, UINT32_MAX
	, BGFX_STENCIL_NONE
	, BGFX_STENCIL_NONE
},
{ // ShadowMap_PackDepthHoriz
	0
	| BGFX_STATE_WRITE_RGB
	| BGFX_STATE_WRITE_A
	| BGFX_STATE_WRITE_Z
	| BGFX_STATE_DEPTH_TEST_LEQUAL
	| BGFX_STATE_CULL_CCW
	| BGFX_STATE_MSAA
	, UINT32_MAX
	, BGFX_STENCIL_TEST_EQUAL
	| BGFX_STENCIL_FUNC_REF(1)
	| BGFX_STENCIL_FUNC_RMASK(0xff)
	| BGFX_STENCIL_OP_FAIL_S_KEEP
	| BGFX_STENCIL_OP_FAIL_Z_KEEP
	| BGFX_STENCIL_OP_PASS_Z_KEEP
	, BGFX_STENCIL_NONE
},
{ // ShadowMap_PackDepthVert
	0
	| BGFX_STATE_WRITE_RGB
	| BGFX_STATE_WRITE_A
	| BGFX_STATE_WRITE_Z
	| BGFX_STATE_DEPTH_TEST_LEQUAL
	| BGFX_STATE_CULL_CCW
	| BGFX_STATE_MSAA
	, UINT32_MAX
	, BGFX_STENCIL_TEST_EQUAL
	| BGFX_STENCIL_FUNC_REF(0)
	| BGFX_STENCIL_FUNC_RMASK(0xff)
	| BGFX_STENCIL_OP_FAIL_S_KEEP
	| BGFX_STENCIL_OP_FAIL_Z_KEEP
	| BGFX_STENCIL_OP_PASS_Z_KEEP
	, BGFX_STENCIL_NONE
},
{ // Custom_BlendLightTexture
	BGFX_STATE_WRITE_RGB
	| BGFX_STATE_WRITE_A
	| BGFX_STATE_WRITE_Z
	| BGFX_STATE_DEPTH_TEST_LEQUAL
	| BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_COLOR, BGFX_STATE_BLEND_INV_SRC_COLOR)
	| BGFX_STATE_CULL_CCW
	| BGFX_STATE_MSAA
	, UINT32_MAX
	, BGFX_STENCIL_NONE
	, BGFX_STENCIL_NONE
},
{ // Custom_DrawPlaneBottom
	BGFX_STATE_WRITE_RGB
	| BGFX_STATE_CULL_CW
	| BGFX_STATE_MSAA
	, UINT32_MAX
	, BGFX_STENCIL_NONE
	, BGFX_STENCIL_NONE
},
};

void FDefaultRendererPrograms::Init()
{
	// Color lighting.
	_colorLighting[EShadowMapType::Single][EDepthImpl::InvZ][EShadowMapImpl::Hard].AttachShader("vs_shadowmaps_color_lighting", "fs_shadowmaps_color_lighting_hard");
	_colorLighting[EShadowMapType::Single][EDepthImpl::InvZ][EShadowMapImpl::PCF].AttachShader("vs_shadowmaps_color_lighting", "fs_shadowmaps_color_lighting_pcf");
	_colorLighting[EShadowMapType::Single][EDepthImpl::InvZ][EShadowMapImpl::VSM].AttachShader("vs_shadowmaps_color_lighting", "fs_shadowmaps_color_lighting_vsm");
	_colorLighting[EShadowMapType::Single][EDepthImpl::InvZ][EShadowMapImpl::ESM].AttachShader("vs_shadowmaps_color_lighting", "fs_shadowmaps_color_lighting_esm");

	_colorLighting[EShadowMapType::Single][EDepthImpl::Linear][EShadowMapImpl::Hard].AttachShader("vs_shadowmaps_color_lighting_linear", "fs_shadowmaps_color_lighting_hard_linear");
	_colorLighting[EShadowMapType::Single][EDepthImpl::Linear][EShadowMapImpl::PCF].AttachShader("vs_shadowmaps_color_lighting_linear", "fs_shadowmaps_color_lighting_pcf_linear");
	_colorLighting[EShadowMapType::Single][EDepthImpl::Linear][EShadowMapImpl::VSM].AttachShader("vs_shadowmaps_color_lighting_linear", "fs_shadowmaps_color_lighting_vsm_linear");
	_colorLighting[EShadowMapType::Single][EDepthImpl::Linear][EShadowMapImpl::ESM].AttachShader("vs_shadowmaps_color_lighting_linear", "fs_shadowmaps_color_lighting_esm_linear");

	_colorLighting[EShadowMapType::Omni][EDepthImpl::InvZ][EShadowMapImpl::Hard].AttachShader("vs_shadowmaps_color_lighting_omni", "fs_shadowmaps_color_lighting_hard_omni");
	_colorLighting[EShadowMapType::Omni][EDepthImpl::InvZ][EShadowMapImpl::PCF].AttachShader("vs_shadowmaps_color_lighting_omni", "fs_shadowmaps_color_lighting_pcf_omni");
	_colorLighting[EShadowMapType::Omni][EDepthImpl::InvZ][EShadowMapImpl::VSM].AttachShader("vs_shadowmaps_color_lighting_omni", "fs_shadowmaps_color_lighting_vsm_omni");
	_colorLighting[EShadowMapType::Omni][EDepthImpl::InvZ][EShadowMapImpl::ESM].AttachShader("vs_shadowmaps_color_lighting_omni", "fs_shadowmaps_color_lighting_esm_omni");

	_colorLighting[EShadowMapType::Omni][EDepthImpl::Linear][EShadowMapImpl::Hard].AttachShader("vs_shadowmaps_color_lighting_linear_omni", "fs_shadowmaps_color_lighting_hard_linear_omni");
	_colorLighting[EShadowMapType::Omni][EDepthImpl::Linear][EShadowMapImpl::PCF].AttachShader("vs_shadowmaps_color_lighting_linear_omni", "fs_shadowmaps_color_lighting_pcf_linear_omni");
	_colorLighting[EShadowMapType::Omni][EDepthImpl::Linear][EShadowMapImpl::VSM].AttachShader("vs_shadowmaps_color_lighting_linear_omni", "fs_shadowmaps_color_lighting_vsm_linear_omni");
	_colorLighting[EShadowMapType::Omni][EDepthImpl::Linear][EShadowMapImpl::ESM].AttachShader("vs_shadowmaps_color_lighting_linear_omni", "fs_shadowmaps_color_lighting_esm_linear_omni");

	_colorLighting[EShadowMapType::Cascade][EDepthImpl::InvZ][EShadowMapImpl::Hard].AttachShader("vs_shadowmaps_color_lighting_csm", "fs_shadowmaps_color_lighting_hard_csm");
	_colorLighting[EShadowMapType::Cascade][EDepthImpl::InvZ][EShadowMapImpl::PCF].AttachShader("vs_shadowmaps_color_lighting_csm", "fs_shadowmaps_color_lighting_pcf_csm");
	_colorLighting[EShadowMapType::Cascade][EDepthImpl::InvZ][EShadowMapImpl::VSM].AttachShader("vs_shadowmaps_color_lighting_csm", "fs_shadowmaps_color_lighting_vsm_csm");
	_colorLighting[EShadowMapType::Cascade][EDepthImpl::InvZ][EShadowMapImpl::ESM].AttachShader("vs_shadowmaps_color_lighting_csm", "fs_shadowmaps_color_lighting_esm_csm");

	_colorLighting[EShadowMapType::Cascade][EDepthImpl::Linear][EShadowMapImpl::Hard].AttachShader("vs_shadowmaps_color_lighting_linear_csm", "fs_shadowmaps_color_lighting_hard_linear_csm");
	_colorLighting[EShadowMapType::Cascade][EDepthImpl::Linear][EShadowMapImpl::PCF].AttachShader("vs_shadowmaps_color_lighting_linear_csm", "fs_shadowmaps_color_lighting_pcf_linear_csm");
	_colorLighting[EShadowMapType::Cascade][EDepthImpl::Linear][EShadowMapImpl::VSM].AttachShader("vs_shadowmaps_color_lighting_linear_csm", "fs_shadowmaps_color_lighting_vsm_linear_csm");
	_colorLighting[EShadowMapType::Cascade][EDepthImpl::Linear][EShadowMapImpl::ESM].AttachShader("vs_shadowmaps_color_lighting_linear_csm", "fs_shadowmaps_color_lighting_esm_linear_csm");
}


}