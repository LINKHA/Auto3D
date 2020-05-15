#pragma once
#include <stdint.h>
#include "Renderer/ShaderProgram.h"

#include <bgfx/bgfx.h>
#define RENDERVIEW_DEFAULT_ID		0
#define RENDERVIEW_SKYBOX_ID		0
#define RENDERVIEW_hdrSkybox	0
#define RENDERVIEW_hdrMesh		1
#define RENDERVIEW_hdrLuminance	2
#define RENDERVIEW_hdrLumScale0	3
#define RENDERVIEW_hdrLumScale1	4
#define RENDERVIEW_hdrLumScale2	5
#define RENDERVIEW_hdrLumScale3	6
#define RENDERVIEW_hdrBrightness	7
#define RENDERVIEW_hdrVBlur	8
#define RENDERVIEW_hdrHBlurTonemap	9
#define RENDERVIEW_SHADOW_BEGIN		11
#define RENDERVIEW_SHADOWMAP_0_ID	12
#define RENDERVIEW_SHADOWMAP_1_ID	13
#define RENDERVIEW_SHADOWMAP_2_ID	14
#define RENDERVIEW_SHADOWMAP_3_ID	15
#define RENDERVIEW_SHADOWMAP_4_ID	16
#define RENDERVIEW_VBLUR_0_ID		17
#define RENDERVIEW_HBLUR_0_ID		18
#define RENDERVIEW_VBLUR_1_ID		19
#define RENDERVIEW_HBLUR_1_ID		20
#define RENDERVIEW_VBLUR_2_ID		21
#define RENDERVIEW_HBLUR_2_ID		22
#define RENDERVIEW_VBLUR_3_ID		23
#define RENDERVIEW_HBLUR_3_ID		24
#define RENDERVIEW_DRAWSCENE_0_ID	25
#define RENDERVIEW_DRAWSCENE_1_ID	26
#define RENDERVIEW_DRAWDEPTH_0_ID	27
#define RENDERVIEW_DRAWDEPTH_1_ID	28
#define RENDERVIEW_DRAWDEPTH_2_ID	29
#define RENDERVIEW_DRAWDEPTH_3_ID	30
#define RENDERVIEW_OCCLUSION_ID		31
#define RENDERVIEW_NO_LIGHT_IBL		32
#define RENDERVIEW_BRDF_LUT_ID		33

namespace Auto3D
{
class FGeometry;

struct FRenderState
{
	enum Enum
	{
		Default = 0,

		ShadowMap_PackDepth,
		ShadowMap_PackDepthHoriz,
		ShadowMap_PackDepthVert,

		Custom_BlendLightTexture,

		Occlusion,
		Count
	};

	uint64_t _state;
	uint32_t _blendFactorRgba;
	uint32_t _fstencil;
	uint32_t _bstencil;

	static FRenderState _renderState[FRenderState::Count];
};


namespace EDepthImpl
{
	enum Data
	{
		InvZ,
		Linear,

		Count
	};
};

namespace EPackDepth
{
	enum Data
	{
		RGBA,
		VSM,

		Count
	};
};

namespace EShadowMapImpl
{
	enum Data
	{
		Hard,
		PCF,
		VSM,
		ESM,

		Count
	};
};

namespace EShadowMapType
{
	enum Data
	{
		Single,
		Omni,
		Cascade,

		Count
	};
};

namespace ERenderInstanceType
{
	enum Data
	{
		STAIC,
		INSTANCE,

		Count
	};
}

struct PosColorTexCoord0Vertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_rgba;
	float m_u;
	float m_v;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();
	}

	static bgfx::VertexLayout ms_layout;
};

void Submit(FGeometry* geometry, uint8_t viewId, float* mtx, bgfx::ProgramHandle program, const FRenderState& renderState = FRenderState::_renderState[FRenderState::Default]);

void SubmitInstance(FGeometry* geometry, uint8_t viewId, bgfx::InstanceDataBuffer* idb, bgfx::ProgramHandle program, const FRenderState& _renderState = FRenderState::_renderState[FRenderState::Default]);

void SubmitOcclusion(FGeometry* geometry, uint8_t viewId, float* mtx, bgfx::ProgramHandle program, const FRenderState& renderState = FRenderState::_renderState[FRenderState::Default]);

void SubmitOcclusionInstace(FGeometry* geometry, uint8_t viewId, bgfx::InstanceDataBuffer* idb, bgfx::ProgramHandle program, const FRenderState& renderState = FRenderState::_renderState[FRenderState::Default]);


void screenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft = true, float _width = 1.0f, float _height = 1.0f);

void setOffsets2x2Lum(bgfx::UniformHandle _handle, uint32_t _width, uint32_t _height);

void setOffsets4x4Lum(bgfx::UniformHandle _handle, uint32_t _width, uint32_t _height);

class AUTO_API FRendererDef
{
public:
	void Init()
	{
		PosColorTexCoord0Vertex::init();
	}
	static float s_texelHalf;
	static bool s_flipV;
};
}
