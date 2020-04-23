#pragma once
#include "AutoConfig.h"
#include "Container/Singleton.h"
#include "Renderer/ShaderProgram.h"
#include "Component/CameraComponent.h"
#include "Component/LightComponent.h"
#include "Component/SkyboxComponent.h"

#include "Platform/ProcessWindow.h"
#include "RHI/bgfx_utils.h"
#include "Resource/Mesh.h"
#include "Resource/ResourceCache.h"
#include "Gameplay/Actor.h"
#include "Component/TransformComponent.h"
#include "RendererDef.h"

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/readerwriter.h>
#include <bx/string.h>

#include "UI/UI.h"
#include <bx/timer.h>

namespace Auto3D
{

struct IBLSettings
{
	IBLSettings()
	{
		m_lightDir[0] = -0.8f;
		m_lightDir[1] = 0.2f;
		m_lightDir[2] = -0.5f;
		m_lightCol[0] = 1.0f;
		m_lightCol[1] = 1.0f;
		m_lightCol[2] = 1.0f;
		m_glossiness = 0.7f;
		m_exposure = 0.0f;
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

	float m_lightDir[3];
	float m_lightCol[3];
	float m_glossiness;
	float m_exposure;
	float m_radianceSlider;
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

		GResourceModule& resourceModule = GResourceModule::Get();

		m_meshBunny = resourceModule.LoadResource<OMesh>("Meshes/bunny.bin");
		m_meshOrb = resourceModule.LoadResource<OMesh>("Meshes/orb.bin");
	}

	void Update(ACameraComponent* camera, ASkyboxComponent* skybox)
	{
		if (camera == nullptr || skybox == nullptr)
			return;

		GProcessWindow& processWindow = GProcessWindow::Get();

		TMatrix4x4F projectionMatrix = camera->GetProjectionMatrix();
		TMatrix4x4F transposeViewMatrix = camera->GetViewMatrix().ToMatrix4().Transpose();
		TVector3F position = camera->GetOwner()->GetTransform()->GetPosition();

		m_uniforms.m_glossiness = m_settings.m_glossiness;
		m_uniforms.m_reflectivity = m_settings.m_reflectivity;
		m_uniforms.m_exposure = m_settings.m_exposure;
		m_uniforms.m_metalOrSpec = float(m_settings.m_metalOrSpec);
		m_uniforms.m_doDiffuse = float(m_settings.m_doDiffuse);
		m_uniforms.m_doSpecular = float(m_settings.m_doSpecular);
		m_uniforms.m_doDiffuseIbl = float(m_settings.m_doDiffuseIbl);
		m_uniforms.m_doSpecularIbl = float(m_settings.m_doSpecularIbl);
		bx::memCopy(m_uniforms.m_rgbDiff, m_settings.m_rgbDiff, 3 * sizeof(float));
		bx::memCopy(m_uniforms.m_rgbSpec, m_settings.m_rgbSpec, 3 * sizeof(float));
		bx::memCopy(m_uniforms.m_lightDir, m_settings.m_lightDir, 3 * sizeof(float));
		bx::memCopy(m_uniforms.m_lightCol, m_settings.m_lightCol, 3 * sizeof(float));

		m_uniforms.m_cameraPos[0] = position._x;
		m_uniforms.m_cameraPos[1] = position._y;
		m_uniforms.m_cameraPos[2] = position._z;

		bgfx::setViewTransform(1, transposeViewMatrix.Data(), projectionMatrix.Data());
		bgfx::setViewRect(1, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));

		// Env mtx.
		float environmentViewMatrix[16];
		camera->GetEnvironmentViewMatrix(environmentViewMatrix);

		// Submit view.
		bx::memCopy(m_uniforms.u_environmentViewMatrix, environmentViewMatrix, 16 * sizeof(float)); // Used for IBL.
		if (0 == m_settings.m_meshSelection)
		{
			// Submit bunny.
			float mtx[16];
			bx::mtxSRT(mtx, 1.0f, 1.0f, 1.0f, 0.0f, bx::kPi, 0.0f, 0.0f, -0.80f, 0.0f);
			m_uniforms._texture = skybox->GetTexture()->GetTextureHandle();
			m_uniforms._textureIrrance = skybox->GetIrranceTexture()->GetTextureHandle();

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
					m_uniforms._texture = skybox->GetTexture()->GetTextureHandle();
					m_uniforms._textureIrrance = skybox->GetIrranceTexture()->GetTextureHandle();

					m_uniforms.submit();
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
			s_texCube = bgfx::createUniform("s_texCube", bgfx::UniformType::Sampler);
			s_texCubeIrr = bgfx::createUniform("s_texCubeIrr", bgfx::UniformType::Sampler);
		}

		void submit()
		{
			bgfx::setUniform(u_params, m_params, NumVec4);
			bgfx::setTexture(0, s_texCube, _texture);
			bgfx::setTexture(1, s_texCubeIrr, _textureIrrance);
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
					float u_environmentViewMatrix[16];
					/* 0*/ struct { float m_mtx0[4]; };
					/* 1*/ struct { float m_mtx1[4]; };
					/* 2*/ struct { float m_mtx2[4]; };
					/* 3*/ struct { float m_mtx3[4]; };
				};
				/* 4*/ struct { float m_glossiness, m_reflectivity, m_exposure, m_unused10[1]; };
				/* 5*/ struct { float m_metalOrSpec, m_unused5[3]; };
				/* 6*/ struct { float m_doDiffuse, m_doSpecular, m_doDiffuseIbl, m_doSpecularIbl; };
				/* 7*/ struct { float m_cameraPos[3], m_unused7[1]; };
				/* 8*/ struct { float m_rgbDiff[4]; };
				/* 9*/ struct { float m_rgbSpec[4]; };
				/*10*/ struct { float m_lightDir[3], m_unused11[1]; };
				/*11*/ struct { float m_lightCol[3], m_unused12[1]; };
			};

			float m_params[NumVec4 * 4];
		};
		bgfx::TextureHandle _texture;
		bgfx::TextureHandle _textureIrrance;

		bgfx::UniformHandle u_params;

		bgfx::UniformHandle s_texCube;
		bgfx::UniformHandle s_texCubeIrr;
	};

	Uniforms m_uniforms;
	IBLSettings m_settings;

	FShaderProgram m_programMesh;


	OMesh* m_meshBunny;
	OMesh* m_meshOrb;

};

}