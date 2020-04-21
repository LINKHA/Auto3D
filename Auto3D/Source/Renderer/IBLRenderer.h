#pragma once
#include "AutoConfig.h"
#include "Container/Singleton.h"
#include "Renderer/ShaderProgram.h"
#include "Component/CameraComponent.h"
#include "Component/LightComponent.h"
#include "Platform/ProcessWindow.h"
#include "RHI/bgfx_utils.h"
#include "Resource/Mesh.h"
#include "Resource/ResourceCache.h"
#include "Gameplay/Actor.h"
#include "Component/TransformComponent.h"

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/readerwriter.h>
#include <bx/string.h>

#include "UI/UI.h"
#include <bx/timer.h>

namespace Auto3D
{
	static float s_texelHalf = 0.0f;

struct LightProbe
{
	enum Enum
	{
		Bolonga,
		Kyoto,

		Count
	};

	void load(const char* _name)
	{
		char filePath[512];

		bx::snprintf(filePath, BX_COUNTOF(filePath), "textures/%s_lod.dds", _name);
		m_tex = loadTexture(filePath, BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);

		bx::snprintf(filePath, BX_COUNTOF(filePath), "textures/%s_irr.dds", _name);
		m_texIrr = loadTexture(filePath, BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);
	}

	void destroy()
	{
		bgfx::destroy(m_tex);
		bgfx::destroy(m_texIrr);
	}

	bgfx::TextureHandle m_tex;
	bgfx::TextureHandle m_texIrr;
};

struct IBLPosColorTexCoord0Vertex
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



void iblScreenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft = false, float _width = 1.0f, float _height = 1.0f);


struct IBLSettings
{
	IBLSettings()
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

		// Vertex declarations.
		IBLPosColorTexCoord0Vertex::init();

		m_lightProbes[LightProbe::Bolonga].load("bolonga");
		m_lightProbes[LightProbe::Kyoto].load("kyoto");
		m_currentLightProbe = LightProbe::Bolonga;

		u_mtx = bgfx::createUniform("u_mtx", bgfx::UniformType::Mat4);
		u_params = bgfx::createUniform("u_params", bgfx::UniformType::Vec4);
		u_flags = bgfx::createUniform("u_flags", bgfx::UniformType::Vec4);
		u_camPos = bgfx::createUniform("u_camPos", bgfx::UniformType::Vec4);
		s_texCube = bgfx::createUniform("s_texCube", bgfx::UniformType::Sampler);
		s_texCubeIrr = bgfx::createUniform("s_texCubeIrr", bgfx::UniformType::Sampler);

		GResourceModule& resourceModule = GResourceModule::Get();

		m_meshBunny = resourceModule.LoadResource<OMesh>("Meshes/bunny.bin");
		m_meshOrb = resourceModule.LoadResource<OMesh>("Meshes/orb.bin");
	}

	void Update(ACameraComponent* camera)
	{
		GProcessWindow& processWindow = GProcessWindow::Get();

		TMatrix4x4F projectionMatrix = camera->GetProjectionMatrix();
		TMatrix4x4F transposeViewMatrix = camera->GetViewMatrix().ToMatrix4().Transpose();
		TVector3F position = camera->GetOwner()->GetTransform()->GetPosition();

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


		int64_t now = bx::getHPCounter();
		static int64_t last = now;
		const int64_t frameTime = now - last;
		last = now;
		const double freq = double(bx::getHPFrequency());
		const float deltaTimeSec = float(double(frameTime) / freq);

		
		//bx::memCopy(m_uniforms.m_cameraPos, &m_camera.m_pos.curr.x, 3 * sizeof(float));
		m_uniforms.m_cameraPos[0] = position._x;
		m_uniforms.m_cameraPos[1] = position._y;
		m_uniforms.m_cameraPos[2] = position._z;

		// View Transform 0.
		float view[16];
		bx::mtxIdentity(view);

		const bgfx::Caps* caps = bgfx::getCaps();

		float proj[16];
		bx::mtxOrtho(proj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0, caps->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);

		// View Transform 1.
		//bx::mtxProj(proj, 45.0f, float(processWindow._width) / float(processWindow._height), 0.1f, 100.0f, caps->homogeneousDepth);
		bgfx::setViewTransform(1, transposeViewMatrix.Data(), projectionMatrix.Data());

		// View rect.
		bgfx::setViewRect(0, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));
		bgfx::setViewRect(1, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));

		// Env rotation.
		const float amount = bx::min(deltaTimeSec / 0.12f, 1.0f);
		m_settings.m_envRotCurr = bx::lerp(m_settings.m_envRotCurr, m_settings.m_envRotDest, amount);

		// Env mtx.
		float mtxEnvView[16];
		camera->envViewMtx(mtxEnvView);
		float mtxEnvRot[16];
		bx::mtxRotateY(mtxEnvRot, m_settings.m_envRotCurr);
		bx::mtxMul(m_uniforms.m_mtx, mtxEnvView, mtxEnvRot); // Used for Skybox.

		// Submit view 0.
		bgfx::setTexture(0, s_texCube, m_lightProbes[m_currentLightProbe].m_tex);
		bgfx::setTexture(1, s_texCubeIrr, m_lightProbes[m_currentLightProbe].m_texIrr);
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		iblScreenSpaceQuad((float)processWindow._width, (float)processWindow._height, true);
		m_uniforms.submit();
		bgfx::submit(0, m_programSky.GetProgram());

		// Submit view 1.
		bx::memCopy(m_uniforms.m_mtx, mtxEnvRot, 16 * sizeof(float)); // Used for IBL.
		if (0 == m_settings.m_meshSelection)
		{
			// Submit bunny.
			float mtx[16];
			bx::mtxSRT(mtx, 1.0f, 1.0f, 1.0f, 0.0f, bx::kPi, 0.0f, 0.0f, -0.80f, 0.0f);
			bgfx::setTexture(0, s_texCube, m_lightProbes[m_currentLightProbe].m_tex);
			bgfx::setTexture(1, s_texCubeIrr, m_lightProbes[m_currentLightProbe].m_texIrr);
			m_uniforms.submit();
			m_meshBunny->submit(1, m_programMesh.GetProgram(), mtx);
		}
		else
		{
			// Submit orbs.
			for (float yy = 0, yend = 5.0f; yy < yend; yy += 1.0f)
			{
				for (float xx = 0, xend = 5.0f; xx < xend; xx += 1.0f)
				{
					const float scale = 1.2f;
					const float spacing = 2.2f;
					const float yAdj = -0.8f;

					float mtx[16];
					bx::mtxSRT(mtx
						, scale / xend
						, scale / xend
						, scale / xend
						, 0.0f
						, 0.0f
						, 0.0f
						, 0.0f + (xx / xend)*spacing - (1.0f + (scale - 1.0f)*0.5f - 1.0f / xend)
						, yAdj / yend + (yy / yend)*spacing - (1.0f + (scale - 1.0f)*0.5f - 1.0f / yend)
						, 0.0f
					);

					m_uniforms.m_glossiness = xx * (1.0f / xend);
					m_uniforms.m_reflectivity = (yend - yy)*(1.0f / yend);
					m_uniforms.m_metalOrSpec = 0.0f;
					m_uniforms.submit();

					bgfx::setTexture(0, s_texCube, m_lightProbes[m_currentLightProbe].m_tex);
					bgfx::setTexture(1, s_texCubeIrr, m_lightProbes[m_currentLightProbe].m_texIrr);
					m_meshOrb->submit(1, m_programMesh.GetProgram(), mtx);
				}
			}
		}

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

	Uniforms m_uniforms;
	IBLSettings m_settings;

	FShaderProgram m_programMesh;
	FShaderProgram m_programSky;

	LightProbe m_lightProbes[LightProbe::Count];
	LightProbe::Enum m_currentLightProbe;

	bgfx::UniformHandle u_mtx;
	bgfx::UniformHandle u_params;
	bgfx::UniformHandle u_flags;
	bgfx::UniformHandle u_camPos;
	bgfx::UniformHandle s_texCube;
	bgfx::UniformHandle s_texCubeIrr;

	OMesh* m_meshBunny;
	OMesh* m_meshOrb;

};

}