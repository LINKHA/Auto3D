#pragma once
#include "AutoConfig.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{

struct LightType
{
	enum Enum
	{
		SpotLight,
		PointLight,
		DirectionalLight,

		Count
	};
};

struct DepthImpl
{
	enum Enum
	{
		InvZ,
		Linear,

		Count
	};
};

struct PackDepth
{
	enum Enum
	{
		RGBA,
		VSM,

		Count
	};
};
struct ShadowMapSettings
{
#define IMGUI_FLOAT_PARAM(_name) float _name, _name##Min, _name##Max, _name##Step
	IMGUI_FLOAT_PARAM(m_sizePwrTwo);
	IMGUI_FLOAT_PARAM(m_depthValuePow);
	IMGUI_FLOAT_PARAM(m_near);
	IMGUI_FLOAT_PARAM(m_far);
	IMGUI_FLOAT_PARAM(m_bias);
	IMGUI_FLOAT_PARAM(m_normalOffset);
	IMGUI_FLOAT_PARAM(m_customParam0);
	IMGUI_FLOAT_PARAM(m_customParam1);
	IMGUI_FLOAT_PARAM(m_xNum);
	IMGUI_FLOAT_PARAM(m_yNum);
	IMGUI_FLOAT_PARAM(m_xOffset);
	IMGUI_FLOAT_PARAM(m_yOffset);
	bool m_doBlur;
	bgfx::ProgramHandle* m_progPack;
	bgfx::ProgramHandle* m_progDraw;
#undef IMGUI_FLOAT_PARAM
};

class AUTO_API FShadowRenderer
{
	FShadowRenderer();
	~FShadowRenderer();
	void init();
	void update();

	ShadowMapSettings m_smSettings[LightType::Count][DepthImpl::Count][SmImpl::Count];
};

}