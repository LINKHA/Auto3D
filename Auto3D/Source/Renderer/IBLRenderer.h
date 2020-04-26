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
		_reflectivity = 0.85f;
		_rgbDiff[0] = 1.0f;
		_rgbDiff[1] = 1.0f;
		_rgbDiff[2] = 1.0f;
		_rgbSpec[0] = 1.0f;
		_rgbSpec[1] = 1.0f;
		_rgbSpec[2] = 1.0f;
		_lod = 0.0f;
		_doDiffuse = false;
		_doSpecular = false;
		_doDiffuseIbl = true;
		_doSpecularIbl = true;
		_metalOrSpec = 0;
		_meshSelection = 0;
	}

	float _lightDirection[3];
	float _lightColor[3];
	float _glossiness;
	float _exposure;
	float _reflectivity;
	float _rgbDiff[3];
	float _rgbSpec[3];
	float _lod;
	bool  _doDiffuse;
	bool  _doSpecular;
	bool  _doDiffuseIbl;
	bool  _doSpecularIbl;
	int32_t _metalOrSpec;
	int32_t _meshSelection;
};


class AUTO_API FIBLRenderer
{
	REGISTER_SINGLETON(FIBLRenderer)
public:
	FIBLRenderer();
	~FIBLRenderer();
	void Init()
	{
		_uniforms.init();
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

		_uniforms._glossiness = _settings._glossiness;
		_uniforms._reflectivity = _settings._reflectivity;
		_uniforms._exposure = _settings._exposure;
		_uniforms._metalOrSpec = float(_settings._metalOrSpec);
		_uniforms._doDiffuse = float(_settings._doDiffuse);
		_uniforms._doSpecular = float(_settings._doSpecular);
		_uniforms._doDiffuseIbl = float(_settings._doDiffuseIbl);
		_uniforms._doSpecularIbl = float(_settings._doSpecularIbl);
		bx::memCopy(_uniforms._rgbDiff, _settings._rgbDiff, 3 * sizeof(float));
		bx::memCopy(_uniforms._rgbSpec, _settings._rgbSpec, 3 * sizeof(float));
		bx::memCopy(_uniforms._lightDirection, _settings._lightDirection, 3 * sizeof(float));
		bx::memCopy(_uniforms._lightColor, _settings._lightColor, 3 * sizeof(float));

		_uniforms._cameraPos[0] = position._x;
		_uniforms._cameraPos[1] = position._y;
		_uniforms._cameraPos[2] = position._z;

		bgfx::setViewTransform(1, transposeViewMatrix.Data(), projectionMatrix.Data());
		bgfx::setViewRect(1, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));

		// Env mtx.
		TMatrix4x4F environmentViewMatrix = camera->GetEnvironmentViewMatrix();

		// Submit view.
		bx::memCopy(_uniforms._environmentViewMatrix, environmentViewMatrix.Data(), 16 * sizeof(float)); // Used for IBL.
		if (0 == _settings._meshSelection)
		{
			// Submit bunny.
			float mtx[16];
			bx::mtxSRT(mtx, 1.0f, 1.0f, 1.0f, 0.0f, bx::kPi, 0.0f, 0.0f, -0.80f, 0.0f);
			_uniforms._texture = skybox->GetTexture()->GetTextureHandle();
			_uniforms._textureIrrance = skybox->GetIrranceTexture()->GetTextureHandle();

			_uniforms.submit();
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

					_uniforms._glossiness = xx * (1.0f / xend);
					_uniforms._reflectivity = (yend - yy)*(1.0f / yend);
					_uniforms._metalOrSpec = 0.0f;
					_uniforms._texture = skybox->GetTexture()->GetTextureHandle();
					_uniforms._textureIrrance = skybox->GetIrranceTexture()->GetTextureHandle();

					_uniforms.submit();
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

	Uniforms _uniforms;
	IBLSettings _settings;

	FShaderProgram m_programMesh;


	OMesh* m_meshBunny;
	OMesh* m_meshOrb;

};

}