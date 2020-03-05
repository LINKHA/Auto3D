#pragma once
#include "AutoConfig.h"
#include "Container/Singleton.h"
#include "RHI/bgfx_utils.h"
#include "Component/LightComponent.h"

#include <bx/math.h>
#include <bgfx/bgfx.h>

#include "Component/CameraComponent.h"

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

	struct SmImpl
	{
		enum Enum
		{
			Hard,
			PCF,
			VSM,
			ESM,

			Count
		};
	};

	struct SmType
	{
		enum Enum
		{
			Single,
			Omni,
			Cascade,

			Count
		};
	};

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

	struct Uniforms
	{
		void init()
		{
			m_ambientPass = 1.0f;
			m_lightingPass = 1.0f;

			m_shadowMapBias = 0.003f;
			m_shadowMapOffset = 0.0f;
			m_shadowMapParam0 = 0.5;
			m_shadowMapParam1 = 1.0;
			m_depthValuePow = 1.0f;
			m_showSmCoverage = 1.0f;
			m_shadowMapTexelSize = 1.0f / 512.0f;

			m_csmFarDistances[0] = 30.0f;
			m_csmFarDistances[1] = 90.0f;
			m_csmFarDistances[2] = 180.0f;
			m_csmFarDistances[3] = 1000.0f;

			m_tetraNormalGreen[0] = 0.0f;
			m_tetraNormalGreen[1] = -0.57735026f;
			m_tetraNormalGreen[2] = 0.81649661f;

			m_tetraNormalYellow[0] = 0.0f;
			m_tetraNormalYellow[1] = -0.57735026f;
			m_tetraNormalYellow[2] = -0.81649661f;

			m_tetraNormalBlue[0] = -0.81649661f;
			m_tetraNormalBlue[1] = 0.57735026f;
			m_tetraNormalBlue[2] = 0.0f;

			m_tetraNormalRed[0] = 0.81649661f;
			m_tetraNormalRed[1] = 0.57735026f;
			m_tetraNormalRed[2] = 0.0f;

			m_XNum = 2.0f;
			m_YNum = 2.0f;
			m_XOffset = 10.0f / 512.0f;
			m_YOffset = 10.0f / 512.0f;

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

		void setPtrs(Material* _materialPtr, ALightComponent* _lightPtr, float* _colorPtr, float* _lightMtxPtr, float* _shadowMapMtx0, float* _shadowMapMtx1, float* _shadowMapMtx2, float* _shadowMapMtx3)
		{
			m_lightMtxPtr = _lightMtxPtr;
			m_colorPtr = _colorPtr;
			m_materialPtr = _materialPtr;
			m_lightPtr = _lightPtr;

			m_shadowMapMtx0 = _shadowMapMtx0;
			m_shadowMapMtx1 = _shadowMapMtx1;
			m_shadowMapMtx2 = _shadowMapMtx2;
			m_shadowMapMtx3 = _shadowMapMtx3;
		}

		// Call this once at initialization.
		void submitConstUniforms()
		{
			bgfx::setUniform(u_tetraNormalGreen, m_tetraNormalGreen);
			bgfx::setUniform(u_tetraNormalYellow, m_tetraNormalYellow);
			bgfx::setUniform(u_tetraNormalBlue, m_tetraNormalBlue);
			bgfx::setUniform(u_tetraNormalRed, m_tetraNormalRed);
		}

		// Call this once per frame.
		void submitPerFrameUniforms()
		{
			bgfx::setUniform(u_params1, m_params1);
			bgfx::setUniform(u_params2, m_params2);
			bgfx::setUniform(u_smSamplingParams, m_paramsBlur);
			bgfx::setUniform(u_csmFarDistances, m_csmFarDistances);

			bgfx::setUniform(u_materialKa, &m_materialPtr->m_ka);
			bgfx::setUniform(u_materialKd, &m_materialPtr->m_kd);
			bgfx::setUniform(u_materialKs, &m_materialPtr->m_ks);

			bgfx::setUniform(u_lightPosition, &m_lightPtr->m_position_viewSpace);
			bgfx::setUniform(u_lightAmbientPower, &m_lightPtr->m_ambientPower);
			bgfx::setUniform(u_lightDiffusePower, &m_lightPtr->m_diffusePower);
			bgfx::setUniform(u_lightSpecularPower, &m_lightPtr->m_specularPower);
			bgfx::setUniform(u_lightSpotDirectionInner, &m_lightPtr->m_spotDirectionInner_viewSpace);
			bgfx::setUniform(u_lightAttenuationSpotOuter, &m_lightPtr->m_attenuationSpotOuter);
		}

		// Call this before each draw call.
		void submitPerDrawUniforms()
		{
			bgfx::setUniform(u_shadowMapMtx0, m_shadowMapMtx0);
			bgfx::setUniform(u_shadowMapMtx1, m_shadowMapMtx1);
			bgfx::setUniform(u_shadowMapMtx2, m_shadowMapMtx2);
			bgfx::setUniform(u_shadowMapMtx3, m_shadowMapMtx3);

			bgfx::setUniform(u_params0, m_params0);
			bgfx::setUniform(u_lightMtx, m_lightMtxPtr);
			bgfx::setUniform(u_color, m_colorPtr);
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
				float m_ambientPass;
				float m_lightingPass;
				float m_unused00;
				float m_unused01;
			};

			float m_params0[4];
		};

		union
		{
			struct
			{
				float m_shadowMapBias;
				float m_shadowMapOffset;
				float m_shadowMapParam0;
				float m_shadowMapParam1;
			};

			float m_params1[4];
		};

		union
		{
			struct
			{
				float m_depthValuePow;
				float m_showSmCoverage;
				float m_shadowMapTexelSize;
				float m_unused23;
			};

			float m_params2[4];
		};

		union
		{
			struct
			{
				float m_XNum;
				float m_YNum;
				float m_XOffset;
				float m_YOffset;
			};

			float m_paramsBlur[4];
		};

		float m_tetraNormalGreen[3];
		float m_tetraNormalYellow[3];
		float m_tetraNormalBlue[3];
		float m_tetraNormalRed[3];
		float m_csmFarDistances[4];

		float* m_lightMtxPtr;
		float* m_colorPtr;
		ALightComponent* m_lightPtr;
		float* m_shadowMapMtx0;
		float* m_shadowMapMtx1;
		float* m_shadowMapMtx2;
		float* m_shadowMapMtx3;
		Material* m_materialPtr;

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

	struct RenderState
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

		uint64_t m_state;
		uint32_t m_blendFactorRgba;
		uint32_t m_fstencil;
		uint32_t m_bstencil;
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

	struct SceneSettings
	{
		LightType::Enum m_lightType;
		DepthImpl::Enum m_depthImpl;
		SmImpl::Enum m_smImpl;
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


	struct Programs
	{
		void init()
		{
			// Misc.
			m_black = loadProgram("vs_shadowmaps_color", "fs_shadowmaps_color_black");
			m_texture = loadProgram("vs_shadowmaps_texture", "fs_shadowmaps_texture");
			m_colorTexture = loadProgram("vs_shadowmaps_color_texture", "fs_shadowmaps_color_texture");

			// Blur.
			m_vBlur[PackDepth::RGBA] = loadProgram("vs_shadowmaps_vblur", "fs_shadowmaps_vblur");
			m_hBlur[PackDepth::RGBA] = loadProgram("vs_shadowmaps_hblur", "fs_shadowmaps_hblur");
			m_vBlur[PackDepth::VSM] = loadProgram("vs_shadowmaps_vblur", "fs_shadowmaps_vblur_vsm");
			m_hBlur[PackDepth::VSM] = loadProgram("vs_shadowmaps_hblur", "fs_shadowmaps_hblur_vsm");

			// Draw depth.
			m_drawDepth[PackDepth::RGBA] = loadProgram("vs_shadowmaps_unpackdepth", "fs_shadowmaps_unpackdepth");
			m_drawDepth[PackDepth::VSM] = loadProgram("vs_shadowmaps_unpackdepth", "fs_shadowmaps_unpackdepth_vsm");

			// Pack depth.
			m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] = loadProgram("vs_shadowmaps_packdepth", "fs_shadowmaps_packdepth");
			m_packDepth[DepthImpl::InvZ][PackDepth::VSM] = loadProgram("vs_shadowmaps_packdepth", "fs_shadowmaps_packdepth_vsm");

			m_packDepth[DepthImpl::Linear][PackDepth::RGBA] = loadProgram("vs_shadowmaps_packdepth_linear", "fs_shadowmaps_packdepth_linear");
			m_packDepth[DepthImpl::Linear][PackDepth::VSM] = loadProgram("vs_shadowmaps_packdepth_linear", "fs_shadowmaps_packdepth_vsm_linear");

			// Color lighting.
			m_colorLighting[SmType::Single][DepthImpl::InvZ][SmImpl::Hard] = loadProgram("vs_shadowmaps_color_lighting", "fs_shadowmaps_color_lighting_hard");
			m_colorLighting[SmType::Single][DepthImpl::InvZ][SmImpl::PCF] = loadProgram("vs_shadowmaps_color_lighting", "fs_shadowmaps_color_lighting_pcf");
			m_colorLighting[SmType::Single][DepthImpl::InvZ][SmImpl::VSM] = loadProgram("vs_shadowmaps_color_lighting", "fs_shadowmaps_color_lighting_vsm");
			m_colorLighting[SmType::Single][DepthImpl::InvZ][SmImpl::ESM] = loadProgram("vs_shadowmaps_color_lighting", "fs_shadowmaps_color_lighting_esm");

			m_colorLighting[SmType::Single][DepthImpl::Linear][SmImpl::Hard] = loadProgram("vs_shadowmaps_color_lighting_linear", "fs_shadowmaps_color_lighting_hard_linear");
			m_colorLighting[SmType::Single][DepthImpl::Linear][SmImpl::PCF] = loadProgram("vs_shadowmaps_color_lighting_linear", "fs_shadowmaps_color_lighting_pcf_linear");
			m_colorLighting[SmType::Single][DepthImpl::Linear][SmImpl::VSM] = loadProgram("vs_shadowmaps_color_lighting_linear", "fs_shadowmaps_color_lighting_vsm_linear");
			m_colorLighting[SmType::Single][DepthImpl::Linear][SmImpl::ESM] = loadProgram("vs_shadowmaps_color_lighting_linear", "fs_shadowmaps_color_lighting_esm_linear");

			m_colorLighting[SmType::Omni][DepthImpl::InvZ][SmImpl::Hard] = loadProgram("vs_shadowmaps_color_lighting_omni", "fs_shadowmaps_color_lighting_hard_omni");
			m_colorLighting[SmType::Omni][DepthImpl::InvZ][SmImpl::PCF] = loadProgram("vs_shadowmaps_color_lighting_omni", "fs_shadowmaps_color_lighting_pcf_omni");
			m_colorLighting[SmType::Omni][DepthImpl::InvZ][SmImpl::VSM] = loadProgram("vs_shadowmaps_color_lighting_omni", "fs_shadowmaps_color_lighting_vsm_omni");
			m_colorLighting[SmType::Omni][DepthImpl::InvZ][SmImpl::ESM] = loadProgram("vs_shadowmaps_color_lighting_omni", "fs_shadowmaps_color_lighting_esm_omni");

			m_colorLighting[SmType::Omni][DepthImpl::Linear][SmImpl::Hard] = loadProgram("vs_shadowmaps_color_lighting_linear_omni", "fs_shadowmaps_color_lighting_hard_linear_omni");
			m_colorLighting[SmType::Omni][DepthImpl::Linear][SmImpl::PCF] = loadProgram("vs_shadowmaps_color_lighting_linear_omni", "fs_shadowmaps_color_lighting_pcf_linear_omni");
			m_colorLighting[SmType::Omni][DepthImpl::Linear][SmImpl::VSM] = loadProgram("vs_shadowmaps_color_lighting_linear_omni", "fs_shadowmaps_color_lighting_vsm_linear_omni");
			m_colorLighting[SmType::Omni][DepthImpl::Linear][SmImpl::ESM] = loadProgram("vs_shadowmaps_color_lighting_linear_omni", "fs_shadowmaps_color_lighting_esm_linear_omni");

			m_colorLighting[SmType::Cascade][DepthImpl::InvZ][SmImpl::Hard] = loadProgram("vs_shadowmaps_color_lighting_csm", "fs_shadowmaps_color_lighting_hard_csm");
			m_colorLighting[SmType::Cascade][DepthImpl::InvZ][SmImpl::PCF] = loadProgram("vs_shadowmaps_color_lighting_csm", "fs_shadowmaps_color_lighting_pcf_csm");
			m_colorLighting[SmType::Cascade][DepthImpl::InvZ][SmImpl::VSM] = loadProgram("vs_shadowmaps_color_lighting_csm", "fs_shadowmaps_color_lighting_vsm_csm");
			m_colorLighting[SmType::Cascade][DepthImpl::InvZ][SmImpl::ESM] = loadProgram("vs_shadowmaps_color_lighting_csm", "fs_shadowmaps_color_lighting_esm_csm");

			m_colorLighting[SmType::Cascade][DepthImpl::Linear][SmImpl::Hard] = loadProgram("vs_shadowmaps_color_lighting_linear_csm", "fs_shadowmaps_color_lighting_hard_linear_csm");
			m_colorLighting[SmType::Cascade][DepthImpl::Linear][SmImpl::PCF] = loadProgram("vs_shadowmaps_color_lighting_linear_csm", "fs_shadowmaps_color_lighting_pcf_linear_csm");
			m_colorLighting[SmType::Cascade][DepthImpl::Linear][SmImpl::VSM] = loadProgram("vs_shadowmaps_color_lighting_linear_csm", "fs_shadowmaps_color_lighting_vsm_linear_csm");
			m_colorLighting[SmType::Cascade][DepthImpl::Linear][SmImpl::ESM] = loadProgram("vs_shadowmaps_color_lighting_linear_csm", "fs_shadowmaps_color_lighting_esm_linear_csm");
		}

		void destroy()
		{
			// Color lighting.
			for (uint8_t ii = 0; ii < SmType::Count; ++ii)
			{
				for (uint8_t jj = 0; jj < DepthImpl::Count; ++jj)
				{
					for (uint8_t kk = 0; kk < SmImpl::Count; ++kk)
					{
						bgfx::destroy(m_colorLighting[ii][jj][kk]);
					}
				}
			}

			// Pack depth.
			for (uint8_t ii = 0; ii < DepthImpl::Count; ++ii)
			{
				for (uint8_t jj = 0; jj < PackDepth::Count; ++jj)
				{
					bgfx::destroy(m_packDepth[ii][jj]);
				}
			}

			// Draw depth.
			for (uint8_t ii = 0; ii < PackDepth::Count; ++ii)
			{
				bgfx::destroy(m_drawDepth[ii]);
			}

			// Hblur.
			for (uint8_t ii = 0; ii < PackDepth::Count; ++ii)
			{
				bgfx::destroy(m_hBlur[ii]);
			}

			// Vblur.
			for (uint8_t ii = 0; ii < PackDepth::Count; ++ii)
			{
				bgfx::destroy(m_vBlur[ii]);
			}

			// Misc.
			bgfx::destroy(m_colorTexture);
			bgfx::destroy(m_texture);
			bgfx::destroy(m_black);
		}

		bgfx::ProgramHandle m_black;
		bgfx::ProgramHandle m_texture;
		bgfx::ProgramHandle m_colorTexture;
		bgfx::ProgramHandle m_vBlur[PackDepth::Count];
		bgfx::ProgramHandle m_hBlur[PackDepth::Count];
		bgfx::ProgramHandle m_drawDepth[PackDepth::Count];
		bgfx::ProgramHandle m_packDepth[DepthImpl::Count][PackDepth::Count];
		bgfx::ProgramHandle m_colorLighting[SmType::Count][DepthImpl::Count][SmImpl::Count];
	};

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

	void screenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft = true, float _width = 1.0f, float _height = 1.0f);

class AUTO_API FShadowRenderer
{
	REGISTER_SINGLETON(FShadowRenderer)
public:

	FShadowRenderer();
	~FShadowRenderer();
	void init();
	void update();
	static uint16_t s_currentShadowMapSize;

	static SceneSettings s_settings;

	static ShadowMapSettings s_smSettings[LightType::Count][DepthImpl::Count][SmImpl::Count];

	static Programs s_programs;

	static Uniforms s_uniforms;


	static bool s_flipV;
	static float s_texelHalf;

	static bgfx::UniformHandle s_texColor;
	static bgfx::UniformHandle s_shadowMap[ShadowMapRenderTargets::Count];

	static bgfx::FrameBufferHandle s_rtShadowMap[ShadowMapRenderTargets::Count];
	static bgfx::FrameBufferHandle s_rtBlur;

	static Material s_defaultMaterial;
	static ALightComponent* s_pointLight;
	static ALightComponent* s_directionalLight;

	static float s_color[4];

	static float s_lightMtx[16];
	static float s_shadowMapMtx[ShadowMapRenderTargets::Count][16];

	static ClearValues s_clearValues;

	static ACameraComponent* s_camera;

	// Compute transform matrices.
	static uint8_t s_shadowMapPasses;
	static float s_lightView[4][16];
	static float s_lightProj[4][16];
	static float s_mtxYpr[4][16];

	static float s_screenProj[16];
	static float s_screenView[16];


	static bgfx::VertexLayout s_posLayout;
};

}