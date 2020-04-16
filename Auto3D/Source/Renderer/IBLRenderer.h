#pragma once
#include "AutoConfig.h"
#include "Container/Singleton.h"
#include "Renderer/ShaderProgram.h"

#include <bgfx/bgfx.h>
#include <bx/bx.h>

namespace Auto3D
{

class AUTO_API FIBLRenderer
{
	REGISTER_SINGLETON(FIBLRenderer)
public:
	FIBLRenderer();
	~FIBLRenderer();
	void Init()
	{
		m_uniforms.init();
		m_programMesh.AttachShader("vs_ibl_mesh", "fs_ibl_mesh");
		m_programSky.AttachShader("vs_ibl_skybox", "fs_ibl_skybox");

	}

	void Update()
	{
		m_uniforms.m_glossiness = m_settings.m_glossiness;
		m_uniforms.m_reflectivity = m_settings.m_reflectivity;
		m_uniforms.m_exposure = m_settings.m_exposure;
		m_uniforms.m_bgType = m_settings.m_bgType;
		m_uniforms.m_metalOrSpec = float(m_settings.m_metalOrSpec);
		m_uniforms.m_doDiffuse = float(m_settings.m_doDiffuse);
		m_uniforms.m_doSpecular = float(m_settings.m_doSpecular);
		m_uniforms.m_doDiffuseIbl = float(m_settings.m_doDiffuseIbl);
		m_uniforms.m_doSpecularIbl = float(m_settings.m_doSpecularIbl);
		bx::memCopy(m_uniforms.m_rgbDiff, m_settings.m_rgbDiff, 3 * sizeof(float));
		bx::memCopy(m_uniforms.m_rgbSpec, m_settings.m_rgbSpec, 3 * sizeof(float));
		bx::memCopy(m_uniforms.m_lightDir, m_settings.m_lightDir, 3 * sizeof(float));
		bx::memCopy(m_uniforms.m_lightCol, m_settings.m_lightCol, 3 * sizeof(float));
	}

	struct Uniforms
	{
		enum { NumVec4 = 12 };

		void init()
		{
			u_params = bgfx::createUniform("u_params", bgfx::UniformType::Vec4, NumVec4);
		}

		void submit()
		{
			bgfx::setUniform(u_params, m_params, NumVec4);
		}

		void destroy()
		{
			bgfx::destroy(u_params);
		}

		union
		{
			struct
			{
				union
				{
					float m_mtx[16];
					/* 0*/ struct { float m_mtx0[4]; };
					/* 1*/ struct { float m_mtx1[4]; };
					/* 2*/ struct { float m_mtx2[4]; };
					/* 3*/ struct { float m_mtx3[4]; };
				};
				/* 4*/ struct { float m_glossiness, m_reflectivity, m_exposure, m_bgType; };
				/* 5*/ struct { float m_metalOrSpec, m_unused5[3]; };
				/* 6*/ struct { float m_doDiffuse, m_doSpecular, m_doDiffuseIbl, m_doSpecularIbl; };
				/* 7*/ struct { float m_cameraPos[3], m_unused7[1]; };
				/* 8*/ struct { float m_rgbDiff[4]; };
				/* 9*/ struct { float m_rgbSpec[4]; };
				/*10*/ struct { float m_lightDir[3], m_unused10[1]; };
				/*11*/ struct { float m_lightCol[3], m_unused11[1]; };
			};

			float m_params[NumVec4 * 4];
		};

		bgfx::UniformHandle u_params;
	};

	struct Settings
	{
		Settings()
		{
			m_envRotCurr = 0.0f;
			m_envRotDest = 0.0f;
			m_lightDir[0] = -0.8f;
			m_lightDir[1] = 0.2f;
			m_lightDir[2] = -0.5f;
			m_lightCol[0] = 1.0f;
			m_lightCol[1] = 1.0f;
			m_lightCol[2] = 1.0f;
			m_glossiness = 0.7f;
			m_exposure = 0.0f;
			m_bgType = 3.0f;
			m_radianceSlider = 2.0f;
			m_reflectivity = 0.85f;
			m_rgbDiff[0] = 1.0f;
			m_rgbDiff[1] = 1.0f;
			m_rgbDiff[2] = 1.0f;
			m_rgbSpec[0] = 1.0f;
			m_rgbSpec[1] = 1.0f;
			m_rgbSpec[2] = 1.0f;
			m_lod = 0.0f;
			m_doDiffuse = false;
			m_doSpecular = false;
			m_doDiffuseIbl = true;
			m_doSpecularIbl = true;
			m_showLightColorWheel = true;
			m_showDiffColorWheel = true;
			m_showSpecColorWheel = true;
			m_metalOrSpec = 0;
			m_meshSelection = 0;
		}

		float m_envRotCurr;
		float m_envRotDest;
		float m_lightDir[3];
		float m_lightCol[3];
		float m_glossiness;
		float m_exposure;
		float m_radianceSlider;
		float m_bgType;
		float m_reflectivity;
		float m_rgbDiff[3];
		float m_rgbSpec[3];
		float m_lod;
		bool  m_doDiffuse;
		bool  m_doSpecular;
		bool  m_doDiffuseIbl;
		bool  m_doSpecularIbl;
		bool  m_showLightColorWheel;
		bool  m_showDiffColorWheel;
		bool  m_showSpecColorWheel;
		int32_t m_metalOrSpec;
		int32_t m_meshSelection;
	};

	Uniforms m_uniforms;
	Settings m_settings;

	FShaderProgram m_programMesh;
	FShaderProgram m_programSky;

};

}