#pragma once
#include <stdint.h>

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
		Custom_DrawPlaneBottom,

		Count
	};

	uint64_t _state;
	uint32_t _blendFactorRgba;
	uint32_t _fstencil;
	uint32_t _bstencil;

	static FRenderState _renderState[FRenderState::Count];
};

}