#pragma once
#include "AutoConfig.h"
#include "Renderer/ShaderProgram.h"
#include "Component/CameraComponent.h"
#include "Component/LightComponent.h"
#include "Component/SkyboxComponent.h"
#include "Renderer/Batch.h"
#include "Math/Vector3.h"

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
class FBatch;

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


class AUTO_API FIBLPipline
{
public:
	FIBLPipline();
	~FIBLPipline();
	void Init();

	void Update(ACameraComponent* camera, ASkyboxComponent* skybox,TVector<FBatch>& batches);

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