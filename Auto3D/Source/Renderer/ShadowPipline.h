#pragma once
#include "AutoConfig.h"
#include "Container/Singleton.h"
#include "RHI/bgfx_utils.h"
#include "Component/LightComponent.h"
#include "Math/Color.h"
#include "Renderer/RendererDef.h"

#include <bx/math.h>
#include <bgfx/bgfx.h>

#include "Component/CameraComponent.h"

namespace Auto3D
{


struct TetrahedronFaces
{
	enum Enum
	{
		Green,
		Yellow,
		Blue,
		Red,

		Count
	}; 
};

struct ProjType
{
	enum Enum
	{
		Horizontal,
		Vertical,

		Count
	};
};

struct ShadowMapRenderTargets
{
	enum Enum
	{
		First,
		Second,
		Third,
		Fourth,

		Count
	};
};

struct PosNormalTexcoordVertex
{
	float    m_x;
	float    m_y;
	float    m_z;
	uint32_t m_normal;
	float    m_u;
	float    m_v;
};

struct Material
{
	union Ambient
	{
		struct
		{
			float m_r;
			float m_g;
			float m_b;
			float m_unused;
		};

		float m_v[4];
	};

	union Diffuse
	{
		struct
		{
			float m_r;
			float m_g;
			float m_b;
			float m_unused;
		};

		float m_v[4];
	};

	union Specular
	{
		struct
		{
			float m_r;
			float m_g;
			float m_b;
			float m_ns;
		};

		float m_v[4];
	};

	Ambient m_ka;
	Diffuse m_kd;
	Specular m_ks;
};


struct ClearValues
{
	ClearValues(uint32_t _clearRgba = 0x30303000
		, float    _clearDepth = 1.0f
		, uint8_t  _clearStencil = 0
	)
		: m_clearRgba(_clearRgba)
		, m_clearDepth(_clearDepth)
		, m_clearStencil(_clearStencil)
	{
	}

	uint32_t m_clearRgba;
	float    m_clearDepth;
	uint8_t  m_clearStencil;
};

struct FShadowMapSettings
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
	FShaderProgram* m_progPack;
	FShaderProgram* m_progPackInstance;

	FShaderProgram* m_progDraw;
	FShaderProgram* m_progDrawInstace;
#undef IMGUI_FLOAT_PARAM
};

struct FShadowSceneSettings
{
	ELightType::Data m_lightType;
	EDepthImpl::Data m_depthImpl;
	EShadowMapImpl::Data m_smImpl;
	float m_spotOuterAngle;
	float m_spotInnerAngle;
	float m_fovXAdjust;
	float m_fovYAdjust;
	float m_coverageSpotL;
	float m_splitDistribution;
	int   m_numSplits;
	bool m_drawDepthBuffer;
	bool m_showSmCoverage;
	bool m_stencilPack;
	bool m_stabilize;
};


void mtxYawPitchRoll(float* __restrict _result
	, float _yaw
	, float _pitch
	, float _roll
);

void splitFrustum(float* _splits, uint8_t _numSplits, float _near, float _far, float _splitWeight = 0.75f);

void worldSpaceFrustumCorners(float* _corners24f
	, float _near
	, float _far
	, float _projWidth
	, float _projHeight
	, const float* __restrict _invViewMtx
);

void mtxBillboard(float* __restrict _result
	, const float* __restrict _view
	, const float* __restrict _pos
	, const float* __restrict _scale);

class AUTO_API FShadowPipline
{
	REGISTER_SINGLETON(FShadowPipline)
public:

	FShadowPipline();
	~FShadowPipline();

	void Init();

	void Update(ACameraComponent* camera, ALightComponent* light);

	void SubmitPerDrawUniforms()
	{
		_uniforms.submitPerDrawUniforms();
	}

	static FShadowMapSettings* GetCurrentShadowMapSettings()
	{
		return &s_shadowMapSettings[s_shadowSceneSettings.m_lightType][s_shadowSceneSettings.m_depthImpl][s_shadowSceneSettings.m_smImpl];
	}
	
	static uint16_t s_currentShadowMapSize;
	static bgfx::UniformHandle s_texColor;
	static bgfx::UniformHandle s_shadowMap[ShadowMapRenderTargets::Count];
	static bgfx::FrameBufferHandle s_rtShadowMap[ShadowMapRenderTargets::Count];
	static bgfx::FrameBufferHandle s_rtBlur;
	static Material s_defaultMaterial;
	static TMatrix4x4F s_lightMtx;
	static TMatrix4x4F s_shadowMapMtx[ShadowMapRenderTargets::Count];
	static ClearValues s_clearValues;
	static TMatrix4x4F s_mtxYpr[4];
	static TMatrix4x4F s_lightView[4];
	static TMatrix4x4F s_lightProj[4];
	static FShadowSceneSettings s_shadowSceneSettings;
	static FShadowMapSettings s_shadowMapSettings[ELightType::Count][EDepthImpl::Count][EShadowMapImpl::Count];

	EShadowMapImpl::Data _shadowMapImpl;
private:

	struct Programs
	{
		void Init()
		{
			// Misc.
			_black.AttachShader("vs_shadowmaps_color", "fs_shadowmaps_color_black");
			_texture.AttachShader("vs_shadowmaps_texture", "fs_shadowmaps_texture");
			_colorTexture.AttachShader("vs_shadowmaps_color_texture", "fs_shadowmaps_color_texture");

			// Blur.
			_vBlur[EPackDepth::RGBA].AttachShader("vs_shadowmaps_vblur", "fs_shadowmaps_vblur");
			_hBlur[EPackDepth::RGBA].AttachShader("vs_shadowmaps_hblur", "fs_shadowmaps_hblur");
			_vBlur[EPackDepth::VSM].AttachShader("vs_shadowmaps_vblur", "fs_shadowmaps_vblur_vsm");
			_hBlur[EPackDepth::VSM].AttachShader("vs_shadowmaps_hblur", "fs_shadowmaps_hblur_vsm");

			// Draw depth.
			_drawDepth[EPackDepth::RGBA].AttachShader("vs_shadowmaps_unpackdepth", "fs_shadowmaps_unpackdepth");
			_drawDepth[EPackDepth::VSM].AttachShader("vs_shadowmaps_unpackdepth", "fs_shadowmaps_unpackdepth_vsm");

			// Pack depth.
			_packDepth[EDepthImpl::InvZ][EPackDepth::RGBA][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_packdepth", "fs_shadowmaps_packdepth");
			_packDepth[EDepthImpl::InvZ][EPackDepth::VSM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_packdepth", "fs_shadowmaps_packdepth_vsm");

			_packDepth[EDepthImpl::Linear][EPackDepth::RGBA][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_packdepth_linear", "fs_shadowmaps_packdepth_linear");
			_packDepth[EDepthImpl::Linear][EPackDepth::VSM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_packdepth_linear", "fs_shadowmaps_packdepth_vsm_linear");


			_packDepth[EDepthImpl::InvZ][EPackDepth::RGBA][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_packdepth_i", "fs_shadowmaps_packdepth");
			_packDepth[EDepthImpl::InvZ][EPackDepth::VSM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_packdepth_i", "fs_shadowmaps_packdepth_vsm");

			_packDepth[EDepthImpl::Linear][EPackDepth::RGBA][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_packdepth_linear_i", "fs_shadowmaps_packdepth_linear");
			_packDepth[EDepthImpl::Linear][EPackDepth::VSM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_packdepth_linear_i", "fs_shadowmaps_packdepth_vsm_linear");

			// Color lighting.
			_colorLighting[EShadowMapType::Single][EDepthImpl::InvZ][EShadowMapImpl::Hard][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting", "fs_shadowmaps_color_lighting_hard");
			_colorLighting[EShadowMapType::Single][EDepthImpl::InvZ][EShadowMapImpl::PCF][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting", "fs_shadowmaps_color_lighting_pcf");
			_colorLighting[EShadowMapType::Single][EDepthImpl::InvZ][EShadowMapImpl::VSM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting", "fs_shadowmaps_color_lighting_vsm");
			_colorLighting[EShadowMapType::Single][EDepthImpl::InvZ][EShadowMapImpl::ESM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting", "fs_shadowmaps_color_lighting_esm");

			_colorLighting[EShadowMapType::Single][EDepthImpl::Linear][EShadowMapImpl::Hard][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_linear", "fs_shadowmaps_color_lighting_hard_linear");
			_colorLighting[EShadowMapType::Single][EDepthImpl::Linear][EShadowMapImpl::PCF][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_linear", "fs_shadowmaps_color_lighting_pcf_linear");
			_colorLighting[EShadowMapType::Single][EDepthImpl::Linear][EShadowMapImpl::VSM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_linear", "fs_shadowmaps_color_lighting_vsm_linear");
			_colorLighting[EShadowMapType::Single][EDepthImpl::Linear][EShadowMapImpl::ESM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_linear", "fs_shadowmaps_color_lighting_esm_linear");

			_colorLighting[EShadowMapType::Omni][EDepthImpl::InvZ][EShadowMapImpl::Hard][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_omni", "fs_shadowmaps_color_lighting_hard_omni");
			_colorLighting[EShadowMapType::Omni][EDepthImpl::InvZ][EShadowMapImpl::PCF][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_omni", "fs_shadowmaps_color_lighting_pcf_omni");
			_colorLighting[EShadowMapType::Omni][EDepthImpl::InvZ][EShadowMapImpl::VSM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_omni", "fs_shadowmaps_color_lighting_vsm_omni");
			_colorLighting[EShadowMapType::Omni][EDepthImpl::InvZ][EShadowMapImpl::ESM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_omni", "fs_shadowmaps_color_lighting_esm_omni");

			_colorLighting[EShadowMapType::Omni][EDepthImpl::Linear][EShadowMapImpl::Hard][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_linear_omni", "fs_shadowmaps_color_lighting_hard_linear_omni");
			_colorLighting[EShadowMapType::Omni][EDepthImpl::Linear][EShadowMapImpl::PCF][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_linear_omni", "fs_shadowmaps_color_lighting_pcf_linear_omni");
			_colorLighting[EShadowMapType::Omni][EDepthImpl::Linear][EShadowMapImpl::VSM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_linear_omni", "fs_shadowmaps_color_lighting_vsm_linear_omni");
			_colorLighting[EShadowMapType::Omni][EDepthImpl::Linear][EShadowMapImpl::ESM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_linear_omni", "fs_shadowmaps_color_lighting_esm_linear_omni");

			_colorLighting[EShadowMapType::Cascade][EDepthImpl::InvZ][EShadowMapImpl::Hard][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_csm", "fs_shadowmaps_color_lighting_hard_csm");
			_colorLighting[EShadowMapType::Cascade][EDepthImpl::InvZ][EShadowMapImpl::PCF][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_csm", "fs_shadowmaps_color_lighting_pcf_csm");
			_colorLighting[EShadowMapType::Cascade][EDepthImpl::InvZ][EShadowMapImpl::VSM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_csm", "fs_shadowmaps_color_lighting_vsm_csm");
			_colorLighting[EShadowMapType::Cascade][EDepthImpl::InvZ][EShadowMapImpl::ESM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_csm", "fs_shadowmaps_color_lighting_esm_csm");

			_colorLighting[EShadowMapType::Cascade][EDepthImpl::Linear][EShadowMapImpl::Hard][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_linear_csm", "fs_shadowmaps_color_lighting_hard_linear_csm");
			_colorLighting[EShadowMapType::Cascade][EDepthImpl::Linear][EShadowMapImpl::PCF][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_linear_csm", "fs_shadowmaps_color_lighting_pcf_linear_csm");
			_colorLighting[EShadowMapType::Cascade][EDepthImpl::Linear][EShadowMapImpl::VSM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_linear_csm", "fs_shadowmaps_color_lighting_vsm_linear_csm");
			_colorLighting[EShadowMapType::Cascade][EDepthImpl::Linear][EShadowMapImpl::ESM][ERenderInstanceType::STAIC].AttachShader("vs_shadowmaps_color_lighting_linear_csm", "fs_shadowmaps_color_lighting_esm_linear_csm");
		

			_colorLighting[EShadowMapType::Single][EDepthImpl::InvZ][EShadowMapImpl::Hard][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_i", "fs_shadowmaps_color_lighting_hard");
			_colorLighting[EShadowMapType::Single][EDepthImpl::InvZ][EShadowMapImpl::PCF][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_i", "fs_shadowmaps_color_lighting_pcf");
			_colorLighting[EShadowMapType::Single][EDepthImpl::InvZ][EShadowMapImpl::VSM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_i", "fs_shadowmaps_color_lighting_vsm");
			_colorLighting[EShadowMapType::Single][EDepthImpl::InvZ][EShadowMapImpl::ESM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_i", "fs_shadowmaps_color_lighting_esm");

			_colorLighting[EShadowMapType::Single][EDepthImpl::Linear][EShadowMapImpl::Hard][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_linear_i", "fs_shadowmaps_color_lighting_hard_linear");
			_colorLighting[EShadowMapType::Single][EDepthImpl::Linear][EShadowMapImpl::PCF][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_linear_i", "fs_shadowmaps_color_lighting_pcf_linear");
			_colorLighting[EShadowMapType::Single][EDepthImpl::Linear][EShadowMapImpl::VSM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_linear_i", "fs_shadowmaps_color_lighting_vsm_linear");
			_colorLighting[EShadowMapType::Single][EDepthImpl::Linear][EShadowMapImpl::ESM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_linear_i", "fs_shadowmaps_color_lighting_esm_linear");

			_colorLighting[EShadowMapType::Omni][EDepthImpl::InvZ][EShadowMapImpl::Hard][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_omni_i", "fs_shadowmaps_color_lighting_hard_omni");
			_colorLighting[EShadowMapType::Omni][EDepthImpl::InvZ][EShadowMapImpl::PCF][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_omni_i", "fs_shadowmaps_color_lighting_pcf_omni");
			_colorLighting[EShadowMapType::Omni][EDepthImpl::InvZ][EShadowMapImpl::VSM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_omni_i", "fs_shadowmaps_color_lighting_vsm_omni");
			_colorLighting[EShadowMapType::Omni][EDepthImpl::InvZ][EShadowMapImpl::ESM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_omni_i", "fs_shadowmaps_color_lighting_esm_omni");

			_colorLighting[EShadowMapType::Omni][EDepthImpl::Linear][EShadowMapImpl::Hard][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_linear_omni_i", "fs_shadowmaps_color_lighting_hard_linear_omni");
			_colorLighting[EShadowMapType::Omni][EDepthImpl::Linear][EShadowMapImpl::PCF][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_linear_omni_i", "fs_shadowmaps_color_lighting_pcf_linear_omni");
			_colorLighting[EShadowMapType::Omni][EDepthImpl::Linear][EShadowMapImpl::VSM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_linear_omni_i", "fs_shadowmaps_color_lighting_vsm_linear_omni");
			_colorLighting[EShadowMapType::Omni][EDepthImpl::Linear][EShadowMapImpl::ESM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_linear_omni_i", "fs_shadowmaps_color_lighting_esm_linear_omni");

			_colorLighting[EShadowMapType::Cascade][EDepthImpl::InvZ][EShadowMapImpl::Hard][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_csm_i", "fs_shadowmaps_color_lighting_hard_csm");
			_colorLighting[EShadowMapType::Cascade][EDepthImpl::InvZ][EShadowMapImpl::PCF][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_csm_i", "fs_shadowmaps_color_lighting_pcf_csm");
			_colorLighting[EShadowMapType::Cascade][EDepthImpl::InvZ][EShadowMapImpl::VSM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_csm_i", "fs_shadowmaps_color_lighting_vsm_csm");
			_colorLighting[EShadowMapType::Cascade][EDepthImpl::InvZ][EShadowMapImpl::ESM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_csm_i", "fs_shadowmaps_color_lighting_esm_csm");

			_colorLighting[EShadowMapType::Cascade][EDepthImpl::Linear][EShadowMapImpl::Hard][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_linear_csm_i", "fs_shadowmaps_color_lighting_hard_linear_csm");
			_colorLighting[EShadowMapType::Cascade][EDepthImpl::Linear][EShadowMapImpl::PCF][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_linear_csm_i", "fs_shadowmaps_color_lighting_pcf_linear_csm");
			_colorLighting[EShadowMapType::Cascade][EDepthImpl::Linear][EShadowMapImpl::VSM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_linear_csm_i", "fs_shadowmaps_color_lighting_vsm_linear_csm");
			_colorLighting[EShadowMapType::Cascade][EDepthImpl::Linear][EShadowMapImpl::ESM][ERenderInstanceType::INSTANCE].AttachShader("vs_shadowmaps_color_lighting_linear_csm_i", "fs_shadowmaps_color_lighting_esm_linear_csm");

		}

		void Destroy()
		{
			// Color lighting.
			for (uint8_t ii = 0; ii < EShadowMapType::Count; ++ii)
			{
				for (uint8_t jj = 0; jj < EDepthImpl::Count; ++jj)
				{
					for (uint8_t kk = 0; kk < EShadowMapImpl::Count; ++kk)
					{
						for (uint8_t pp = 0; pp < ERenderInstanceType::Count; ++pp)
						{
							_colorLighting[ii][jj][kk][pp].Release();
						}
					}
				}
			}

			// Pack depth.
			for (uint8_t ii = 0; ii < EDepthImpl::Count; ++ii)
			{
				for (uint8_t jj = 0; jj < EPackDepth::Count; ++jj)
				{
					for (uint8_t kk = 0; kk < ERenderInstanceType::Count; ++kk)
					{
						_packDepth[ii][jj][kk].Release();
					}
				}
			}

			// Draw depth.
			for (uint8_t ii = 0; ii < EPackDepth::Count; ++ii)
			{
				_drawDepth[ii].Release();
			}

			// Hblur.
			for (uint8_t ii = 0; ii < EPackDepth::Count; ++ii)
			{
				_hBlur[ii].Release();
			}

			// Vblur.
			for (uint8_t ii = 0; ii < EPackDepth::Count; ++ii)
			{
				_vBlur[ii].Release();
			}

			// Misc.
			_colorTexture.Release();
			_texture.Release();
			_black.Release();
		}

		FShaderProgram _black;
		FShaderProgram _texture;
		FShaderProgram _colorTexture;
		FShaderProgram _vBlur[EPackDepth::Count];
		FShaderProgram _hBlur[EPackDepth::Count];
		FShaderProgram _drawDepth[EPackDepth::Count];
		FShaderProgram _packDepth[EDepthImpl::Count][EPackDepth::Count][ERenderInstanceType::Count];
		FShaderProgram _colorLighting[EShadowMapType::Count][EDepthImpl::Count][EShadowMapImpl::Count][ERenderInstanceType::Count];
	};

	struct Uniforms
	{
		void init()
		{
			_ambientPass = 1.0f;
			_lightingPass = 1.0f;

			_shadowMapBias = 0.003f;
			_shadowMapOffset = 0.0f;
			_shadowMapParam0 = 0.5;
			_shadowMapParam1 = 1.0;
			_depthValuePow = 1.0f;
			_showSmCoverage = 1.0f;
			_shadowMapTexelSize = 1.0f / 512.0f;

			_csmFarDistances[0] = 30.0f;
			_csmFarDistances[1] = 90.0f;
			_csmFarDistances[2] = 180.0f;
			_csmFarDistances[3] = 1000.0f;

			_tetraNormalGreen[0] = 0.0f;
			_tetraNormalGreen[1] = -0.57735026f;
			_tetraNormalGreen[2] = 0.81649661f;

			_tetraNormalYellow[0] = 0.0f;
			_tetraNormalYellow[1] = -0.57735026f;
			_tetraNormalYellow[2] = -0.81649661f;

			_tetraNormalBlue[0] = -0.81649661f;
			_tetraNormalBlue[1] = 0.57735026f;
			_tetraNormalBlue[2] = 0.0f;

			_tetraNormalRed[0] = 0.81649661f;
			_tetraNormalRed[1] = 0.57735026f;
			_tetraNormalRed[2] = 0.0f;

			_XNum = 2.0f;
			_YNum = 2.0f;
			_XOffset = 10.0f / 512.0f;
			_YOffset = 10.0f / 512.0f;

			u_params0 = bgfx::createUniform("u_params0", bgfx::UniformType::Vec4);
			u_params1 = bgfx::createUniform("u_params1", bgfx::UniformType::Vec4);
			u_params2 = bgfx::createUniform("u_params2", bgfx::UniformType::Vec4);
			u_color = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
			u_smSamplingParams = bgfx::createUniform("u_smSamplingParams", bgfx::UniformType::Vec4);
			u_csmFarDistances = bgfx::createUniform("u_csmFarDistances", bgfx::UniformType::Vec4);
			u_lightMtx = bgfx::createUniform("u_lightMtx", bgfx::UniformType::Mat4);

			u_tetraNormalGreen = bgfx::createUniform("u_tetraNormalGreen", bgfx::UniformType::Vec4);
			u_tetraNormalYellow = bgfx::createUniform("u_tetraNormalYellow", bgfx::UniformType::Vec4);
			u_tetraNormalBlue = bgfx::createUniform("u_tetraNormalBlue", bgfx::UniformType::Vec4);
			u_tetraNormalRed = bgfx::createUniform("u_tetraNormalRed", bgfx::UniformType::Vec4);

			u_shadowMapMtx0 = bgfx::createUniform("u_shadowMapMtx0", bgfx::UniformType::Mat4);
			u_shadowMapMtx1 = bgfx::createUniform("u_shadowMapMtx1", bgfx::UniformType::Mat4);
			u_shadowMapMtx2 = bgfx::createUniform("u_shadowMapMtx2", bgfx::UniformType::Mat4);
			u_shadowMapMtx3 = bgfx::createUniform("u_shadowMapMtx3", bgfx::UniformType::Mat4);

			u_lightPosition = bgfx::createUniform("u_lightPosition", bgfx::UniformType::Vec4);
			u_lightAmbientPower = bgfx::createUniform("u_lightAmbientPower", bgfx::UniformType::Vec4);
			u_lightDiffusePower = bgfx::createUniform("u_lightDiffusePower", bgfx::UniformType::Vec4);
			u_lightSpecularPower = bgfx::createUniform("u_lightSpecularPower", bgfx::UniformType::Vec4);
			u_lightSpotDirectionInner = bgfx::createUniform("u_lightSpotDirectionInner", bgfx::UniformType::Vec4);
			u_lightAttenuationSpotOuter = bgfx::createUniform("u_lightAttenuationSpotOuter", bgfx::UniformType::Vec4);

			u_materialKa = bgfx::createUniform("u_materialKa", bgfx::UniformType::Vec4);
			u_materialKd = bgfx::createUniform("u_materialKd", bgfx::UniformType::Vec4);
			u_materialKs = bgfx::createUniform("u_materialKs", bgfx::UniformType::Vec4);

		}

		void setPtrs(Material* materialPtr, ALightComponent* lightPtr, float* colorPtr, float* lightMtxPtr, float* shadowMapMtx0, float* shadowMapMtx1, float* shadowMapMtx2, float* shadowMapMtx3)
		{
			_lightMtxPtr = lightMtxPtr;
			_colorPtr = colorPtr;
			_materialPtr = materialPtr;
			_lightPtr = lightPtr;

			_shadowMapMtx0 = shadowMapMtx0;
			_shadowMapMtx1 = shadowMapMtx1;
			_shadowMapMtx2 = shadowMapMtx2;
			_shadowMapMtx3 = shadowMapMtx3;
		}

		// Call this once at initialization.
		void submitConstUniforms()
		{
			bgfx::setUniform(u_tetraNormalGreen, _tetraNormalGreen);
			bgfx::setUniform(u_tetraNormalYellow, _tetraNormalYellow);
			bgfx::setUniform(u_tetraNormalBlue, _tetraNormalBlue);
			bgfx::setUniform(u_tetraNormalRed, _tetraNormalRed);
		}

		// Call this once per frame.
		void submitPerFrameUniforms()
		{
			bgfx::setUniform(u_params1, m_params1);
			bgfx::setUniform(u_params2, m_params2);
			bgfx::setUniform(u_smSamplingParams, m_paramsBlur);
			bgfx::setUniform(u_csmFarDistances, _csmFarDistances);

			bgfx::setUniform(u_materialKa, &_materialPtr->m_ka);
			bgfx::setUniform(u_materialKd, &_materialPtr->m_kd);
			bgfx::setUniform(u_materialKs, &_materialPtr->m_ks);

			bgfx::setUniform(u_lightPosition, &_lightPtr->m_position_viewSpace);
			bgfx::setUniform(u_lightAmbientPower, &_lightPtr->m_ambientPower);
			bgfx::setUniform(u_lightDiffusePower, &_lightPtr->m_diffusePower);
			bgfx::setUniform(u_lightSpecularPower, &_lightPtr->m_specularPower);
			bgfx::setUniform(u_lightSpotDirectionInner, &_lightPtr->m_spotDirectionInner_viewSpace);
			bgfx::setUniform(u_lightAttenuationSpotOuter, &_lightPtr->m_attenuationSpotOuter);
		}

		// Call this before each draw call.
		void submitPerDrawUniforms()
		{
			bgfx::setUniform(u_shadowMapMtx0, _shadowMapMtx0);
			bgfx::setUniform(u_shadowMapMtx1, _shadowMapMtx1);
			bgfx::setUniform(u_shadowMapMtx2, _shadowMapMtx2);
			bgfx::setUniform(u_shadowMapMtx3, _shadowMapMtx3);

			bgfx::setUniform(u_params0, m_params0);
			bgfx::setUniform(u_lightMtx, _lightMtxPtr);
			bgfx::setUniform(u_color, _colorPtr);
		}

		void destroy()
		{
			bgfx::destroy(u_params0);
			bgfx::destroy(u_params1);
			bgfx::destroy(u_params2);
			bgfx::destroy(u_color);
			bgfx::destroy(u_smSamplingParams);
			bgfx::destroy(u_csmFarDistances);

			bgfx::destroy(u_materialKa);
			bgfx::destroy(u_materialKd);
			bgfx::destroy(u_materialKs);

			bgfx::destroy(u_tetraNormalGreen);
			bgfx::destroy(u_tetraNormalYellow);
			bgfx::destroy(u_tetraNormalBlue);
			bgfx::destroy(u_tetraNormalRed);

			bgfx::destroy(u_shadowMapMtx0);
			bgfx::destroy(u_shadowMapMtx1);
			bgfx::destroy(u_shadowMapMtx2);
			bgfx::destroy(u_shadowMapMtx3);

			bgfx::destroy(u_lightMtx);
			bgfx::destroy(u_lightPosition);
			bgfx::destroy(u_lightAmbientPower);
			bgfx::destroy(u_lightDiffusePower);
			bgfx::destroy(u_lightSpecularPower);
			bgfx::destroy(u_lightSpotDirectionInner);
			bgfx::destroy(u_lightAttenuationSpotOuter);
		}

		union
		{
			struct
			{
				float _ambientPass;
				float _lightingPass;
				float m_unused00;
				float m_unused01;
			};

			float m_params0[4];
		};

		union
		{
			struct
			{
				float _shadowMapBias;
				float _shadowMapOffset;
				float _shadowMapParam0;
				float _shadowMapParam1;
			};

			float m_params1[4];
		};

		union
		{
			struct
			{
				float _depthValuePow;
				float _showSmCoverage;
				float _shadowMapTexelSize;
				float m_unused23;
			};

			float m_params2[4];
		};

		union
		{
			struct
			{
				float _XNum;
				float _YNum;
				float _XOffset;
				float _YOffset;
			};

			float m_paramsBlur[4];
		};

		float _tetraNormalGreen[3];
		float _tetraNormalYellow[3];
		float _tetraNormalBlue[3];
		float _tetraNormalRed[3];
		float _csmFarDistances[4];

		float* _lightMtxPtr;
		float* _colorPtr;
		ALightComponent* _lightPtr;
		float* _shadowMapMtx0;
		float* _shadowMapMtx1;
		float* _shadowMapMtx2;
		float* _shadowMapMtx3;
		Material* _materialPtr;

	private:
		bgfx::UniformHandle u_params0;
		bgfx::UniformHandle u_params1;
		bgfx::UniformHandle u_params2;
		bgfx::UniformHandle u_color;
		bgfx::UniformHandle u_smSamplingParams;
		bgfx::UniformHandle u_csmFarDistances;

		bgfx::UniformHandle u_materialKa;
		bgfx::UniformHandle u_materialKd;
		bgfx::UniformHandle u_materialKs;

		bgfx::UniformHandle u_tetraNormalGreen;
		bgfx::UniformHandle u_tetraNormalYellow;
		bgfx::UniformHandle u_tetraNormalBlue;
		bgfx::UniformHandle u_tetraNormalRed;

		bgfx::UniformHandle u_shadowMapMtx0;
		bgfx::UniformHandle u_shadowMapMtx1;
		bgfx::UniformHandle u_shadowMapMtx2;
		bgfx::UniformHandle u_shadowMapMtx3;

		bgfx::UniformHandle u_lightMtx;
		bgfx::UniformHandle u_lightPosition;
		bgfx::UniformHandle u_lightAmbientPower;
		bgfx::UniformHandle u_lightDiffusePower;
		bgfx::UniformHandle u_lightSpecularPower;
		bgfx::UniformHandle u_lightSpotDirectionInner;
		bgfx::UniformHandle u_lightAttenuationSpotOuter;
	};

	Programs _programs;

	Uniforms _uniforms;


	TMatrix4x4F _screenProj;
	TMatrix4x4F _screenView;

	bgfx::VertexLayout _shadowPosLayout;

};

}