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
		_lightDirection[0] = -0.8f;
		_lightDirection[1] = 0.2f;
		_lightDirection[2] = -0.5f;
		_lightColor[0] = 1.0f;
		_lightColor[1] = 1.0f;
		_lightColor[2] = 1.0f;
		_glossiness = 0.7f;
		_exposure = 0.0f;
		m_radianceSlider = 2.0f;
		_reflectivity = 0.85f;
		_rgbDiff[0] = 1.0f;
		_rgbDiff[1] = 1.0f;
		_rgbDiff[2] = 1.0f;
		_rgbSpec[0] = 1.0f;
		_rgbSpec[1] = 1.0f;
		_rgbSpec[2] = 1.0f;
		m_lod = 0.0f;
		_doDiffuse = false;
		_doSpecular = false;
		_doDiffuseIbl = true;
		_doSpecularIbl = true;
		m_showLightColorWheel = true;
		m_showDiffColorWheel = true;
		m_showSpecColorWheel = true;
		_metalOrSpec = 0;
		m_meshSelection = 0;
	}

	float _lightDirection[3];
	float _lightColor[3];
	float _glossiness;
	float _exposure;
	float m_radianceSlider;
	float _reflectivity;
	float _rgbDiff[3];
	float _rgbSpec[3];
	float m_lod;
	bool  _doDiffuse;
	bool  _doSpecular;
	bool  _doDiffuseIbl;
	bool  _doSpecularIbl;
	bool  m_showLightColorWheel;
	bool  m_showDiffColorWheel;
	bool  m_showSpecColorWheel;
	int32_t _metalOrSpec;
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

		m_uniforms._glossiness = m_settings._glossiness;
		m_uniforms._reflectivity = m_settings._reflectivity;
		m_uniforms._exposure = m_settings._exposure;
		m_uniforms._metalOrSpec = float(m_settings._metalOrSpec);
		m_uniforms._doDiffuse = float(m_settings._doDiffuse);
		m_uniforms._doSpecular = float(m_settings._doSpecular);
		m_uniforms._doDiffuseIbl = float(m_settings._doDiffuseIbl);
		m_uniforms._doSpecularIbl = float(m_settings._doSpecularIbl);
		bx::memCopy(m_uniforms._rgbDiff, m_settings._rgbDiff, 3 * sizeof(float));
		bx::memCopy(m_uniforms._rgbSpec, m_settings._rgbSpec, 3 * sizeof(float));
		bx::memCopy(m_uniforms._lightDirection, m_settings._lightDirection, 3 * sizeof(float));
		bx::memCopy(m_uniforms._lightColor, m_settings._lightColor, 3 * sizeof(float));

		m_uniforms._cameraPos[0] = position._x;
		m_uniforms._cameraPos[1] = position._y;
		m_uniforms._cameraPos[2] = position._z;

		bgfx::setViewTransform(1, transposeViewMatrix.Data(), projectionMatrix.Data());
		bgfx::setViewRect(1, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));

		// Env mtx.
		TMatrix4x4F environmentViewMatrix = camera->GetEnvironmentViewMatrix();

		// Submit view.
		bx::memCopy(m_uniforms._environmentViewMatrix, environmentViewMatrix.Data(), 16 * sizeof(float)); // Used for IBL.
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

					m_uniforms._glossiness = xx * (1.0f / xend);
					m_uniforms._reflectivity = (yend - yy)*(1.0f / yend);
					m_uniforms._metalOrSpec = 0.0f;
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
			bgfx::setUniform(u_params, _params, NumVec4);
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
					float _environmentViewMatrix[16];
					/* 0*/ struct { float _mtx0[4]; };
					/* 1*/ struct { float _mtx1[4]; };
					/* 2*/ struct { float _mtx2[4]; };
					/* 3*/ struct { float _mtx3[4]; };
				};
				/* 4*/ struct { float _glossiness, _reflectivity, _exposure, _unused0[1]; };
				/* 5*/ struct { float _metalOrSpec, _unused1[3]; };
				/* 6*/ struct { float _doDiffuse, _doSpecular, _doDiffuseIbl, _doSpecularIbl; };
				/* 7*/ struct { float _cameraPos[3], _unused2[1]; };
				/* 8*/ struct { float _rgbDiff[4]; };
				/* 9*/ struct { float _rgbSpec[4]; };
				/*10*/ struct { float _lightDirection[3], _unused3[1]; };
				/*11*/ struct { float _lightColor[3], _unused4[1]; };
			};

			float _params[NumVec4 * 4];
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