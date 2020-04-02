#pragma once
#include <stdint.h>
#include "Renderer/ShaderProgram.h"
#define RENDERVIEW_SHADOWMAP_0_ID 1
#define RENDERVIEW_SHADOWMAP_1_ID 2
#define RENDERVIEW_SHADOWMAP_2_ID 3
#define RENDERVIEW_SHADOWMAP_3_ID 4
#define RENDERVIEW_SHADOWMAP_4_ID 5
#define RENDERVIEW_VBLUR_0_ID     6
#define RENDERVIEW_HBLUR_0_ID     7
#define RENDERVIEW_VBLUR_1_ID     8
#define RENDERVIEW_HBLUR_1_ID     9
#define RENDERVIEW_VBLUR_2_ID     10
#define RENDERVIEW_HBLUR_2_ID     11
#define RENDERVIEW_VBLUR_3_ID     12
#define RENDERVIEW_HBLUR_3_ID     13
#define RENDERVIEW_DRAWSCENE_0_ID 14
#define RENDERVIEW_DRAWSCENE_1_ID 15
#define RENDERVIEW_DRAWDEPTH_0_ID 16
#define RENDERVIEW_DRAWDEPTH_1_ID 17
#define RENDERVIEW_DRAWDEPTH_2_ID 18
#define RENDERVIEW_DRAWDEPTH_3_ID 19

namespace Auto3D
{

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
struct FDefaultRendererPrograms
{
	void Init();
};

}