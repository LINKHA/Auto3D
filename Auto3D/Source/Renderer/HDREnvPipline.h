#pragma once
#include "AutoConfig.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/RendererDef.h"
#include "Platform/ProcessWindow.h"
#include "Math/Matrix4x4.h"
#include "Component/CameraComponent.h"
#include "Component/SkyboxComponent.h"
#include "Renderer/RendererDef.h"

#include <bgfx/bgfx.h>
#include <bx/math.h>

namespace Auto3D
{

class AUTO_API FHDREnvPipline
{
public:
	FHDREnvPipline() {}
	~FHDREnvPipline() {}

	void Init()
	{
		_equirectangularToCubemap.AttachShader("vs_cube_map", "fs_equirectangular_to_cubemap");
		_irradianceConvolution.AttachShader("vs_cube_map", "fs_irradiance_convolution");
		/*_prefilter.AttachShader("vs_cube_map", "fs_prefilter");*/

		

		_uniforms.Init();
	}

	void Update(ACameraComponent* camera,ASkyboxComponent* skybox)
	{
		GProcessWindow& processWindow = GProcessWindow::Get();

		TMatrix4x4F environmentViewMatrix = camera->GetEnvironmentViewMatrix();
		_uniforms._environmentViewMatrix = environmentViewMatrix.Transpose();
		_uniforms._environmentViewTexture = skybox->GetHDRTexture()->GetTextureHandle();

		for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._environmentViewTextureCubeFaceFb); ++ii)
		{
			bgfx::ViewId viewId = bgfx::ViewId(ii + 1);
			bgfx::setViewFrameBuffer(viewId, _uniforms._environmentViewTextureCubeFaceFb[ii]);

			bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR);
			bgfx::setViewRect(viewId, 0, 0, 512, 512);

			TMatrix4x4F captureProjection = Perspective(90.0f, 1.0f, 0.1f, 10.0f);
			TMatrix4x4F captureViews[] =
			{
				LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(1.0f,  0.0f,  0.0f), TVector3F(0.0f, -1.0f,  0.0f)),
				LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(-1.0f,  0.0f,  0.0f), TVector3F(0.0f, -1.0f,  0.0f)),
				LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f,  1.0f,  0.0f), TVector3F(0.0f,  0.0f,  1.0f)),
				LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f, -1.0f,  0.0f), TVector3F(0.0f,  0.0f, -1.0f)),
				LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f,  0.0f,  1.0f), TVector3F(0.0f, -1.0f,  0.0f)),
				LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f,  0.0f, -1.0f), TVector3F(0.0f, -1.0f,  0.0f))
			};

			bgfx::setViewTransform(viewId, captureViews[ii].Data(), captureProjection.Data());
			bgfx::setViewRect(viewId, 0, 0, uint16_t(512), uint16_t(512));

			static OMesh* cubeMesh = GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin");
			static FGeometry* cubeGeometry = cubeMesh->CreateGeometry();

			_uniforms.Submit();

			Submit(cubeGeometry, viewId, NULL, _equirectangularToCubemap.GetProgram());
		}


		for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._irradianceViewTextureCubeFaceFb); ++ii)
		{
			bgfx::ViewId viewId = bgfx::ViewId(ii + 7);
			bgfx::setViewFrameBuffer(viewId, _uniforms._irradianceViewTextureCubeFaceFb[ii]);

			bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR);
			bgfx::setViewRect(viewId, 0, 0, 512, 512);

			TMatrix4x4F captureProjection = Perspective(90.0f, 1.0f, 0.1f, 10.0f);
			TMatrix4x4F captureViews[] =
			{
				LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(1.0f,  0.0f,  0.0f), TVector3F(0.0f, -1.0f,  0.0f)),
				LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(-1.0f,  0.0f,  0.0f), TVector3F(0.0f, -1.0f,  0.0f)),
				LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f,  1.0f,  0.0f), TVector3F(0.0f,  0.0f,  1.0f)),
				LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f, -1.0f,  0.0f), TVector3F(0.0f,  0.0f, -1.0f)),
				LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f,  0.0f,  1.0f), TVector3F(0.0f, -1.0f,  0.0f)),
				LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f,  0.0f, -1.0f), TVector3F(0.0f, -1.0f,  0.0f))
			};

			bgfx::setViewTransform(viewId, captureViews[ii].Data(), captureProjection.Data());
			bgfx::setViewRect(viewId, 0, 0, uint16_t(512), uint16_t(512));

			static OMesh* cubeMesh = GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin");
			static FGeometry* cubeGeometry = cubeMesh->CreateGeometry();

			//_uniforms.Submit();
			bgfx::setTexture(0, _uniforms.us_equirectangulaCubeMap, _uniforms._environmentViewTextureCube);
			Submit(cubeGeometry, viewId, NULL, _irradianceConvolution.GetProgram());
		}



	}
	struct Uniforms
	{
		void Init()
		{
			us_equirectangularMap = bgfx::createUniform("s_equirectangularMap", bgfx::UniformType::Sampler);
			//us_equirectangulaCubeMap = bgfx::createUniform("s_equirectangulaCubeMap", bgfx::UniformType::Sampler);

			_environmentViewTextureCube = bgfx::createTextureCube(512, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT);
			for (uint32_t ii = 0; ii < BX_COUNTOF(_environmentViewTextureCubeFaceFb); ++ii)
			{
				bgfx::Attachment at;
				at.init(_environmentViewTextureCube, bgfx::Access::Write, uint16_t(ii));
				_environmentViewTextureCubeFaceFb[ii] = bgfx::createFrameBuffer(1, &at);
			}

			_irradianceViewTextureCube = bgfx::createTextureCube(512, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT);
			for (uint32_t ii = 0; ii < BX_COUNTOF(_environmentViewTextureCubeFaceFb); ++ii)
			{
				bgfx::Attachment at;
				at.init(_irradianceViewTextureCube, bgfx::Access::Write, uint16_t(ii));
				_irradianceViewTextureCubeFaceFb[ii] = bgfx::createFrameBuffer(1, &at);
			}
		}

		void Update()
		{
		}

		void Submit()
		{
			bgfx::setTexture(0, us_equirectangularMap, _environmentViewTexture);
		}

		void Destroy()
		{
		}

		TMatrix4x4F _environmentViewMatrix;
		bgfx::TextureHandle _environmentViewTexture;

		bgfx::TextureHandle _environmentViewTextureCube;
		bgfx::FrameBufferHandle _environmentViewTextureCubeFaceFb[6];

		bgfx::UniformHandle us_equirectangularMap;
		bgfx::UniformHandle us_equirectangulaCubeMap;

		bgfx::TextureHandle _irradianceViewTextureCube;
		bgfx::FrameBufferHandle _irradianceViewTextureCubeFaceFb[6];
	};
	
	Uniforms _uniforms;

	FShaderProgram _equirectangularToCubemap;
	FShaderProgram _irradianceConvolution;
	FShaderProgram _prefilter;

};

}