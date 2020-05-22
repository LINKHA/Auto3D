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
		_prefilter.AttachShader("vs_cube_map", "fs_prefilter");

		

		_uniforms.Init();
	}

	void Update(ACameraComponent* camera, ASkyboxComponent* skybox)
	{
		GProcessWindow& processWindow = GProcessWindow::Get();

		TMatrix4x4F environmentViewMatrix = camera->GetEnvironmentViewMatrix();
		_uniforms._environmentViewMatrix = environmentViewMatrix.Transpose();
		_uniforms._environmentViewTexture = skybox->GetHDRTexture()->GetTextureHandle();

		static TMatrix4x4F captureProjection = Perspective(90.0f, 1.0f, 0.1f, 10.0f);
		static TMatrix4x4F captureViews[] =
		{
			LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(1.0f,  0.0f,  0.0f), TVector3F(0.0f, -1.0f,  0.0f)),
			LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(-1.0f,  0.0f,  0.0f), TVector3F(0.0f, -1.0f,  0.0f)),
			LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f,  1.0f,  0.0f), TVector3F(0.0f,  0.0f,  1.0f)),
			LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f, -1.0f,  0.0f), TVector3F(0.0f,  0.0f, -1.0f)),
			LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f,  0.0f,  1.0f), TVector3F(0.0f, -1.0f,  0.0f)),
			LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f,  0.0f, -1.0f), TVector3F(0.0f, -1.0f,  0.0f))
		};

		static OMesh* cubeMesh = GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin");
		static FGeometry* cubeGeometry = cubeMesh->CreateGeometry();


		static int sss = 0;

		if (!sss)
		{
			sss = 1;

			for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._environmentViewTextureCubeFaceFb); ++ii)
			{
				bgfx::ViewId viewId = bgfx::ViewId(ii + 1);
				bgfx::setViewFrameBuffer(viewId, _uniforms._environmentViewTextureCubeFaceFb[ii]);
				bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR);
				bgfx::setViewRect(viewId, 0, 0, 512, 512);
				bgfx::setViewTransform(viewId, captureViews[ii].Data(), captureProjection.Data());

				_uniforms.Submit();

				Submit(cubeGeometry, viewId, NULL, _equirectangularToCubemap.GetProgram());
			}


			for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._irradianceViewTextureCubeFaceFb); ++ii)
			{
				bgfx::ViewId viewId = bgfx::ViewId(ii + 7);
				bgfx::setViewFrameBuffer(viewId, _uniforms._irradianceViewTextureCubeFaceFb[ii]);
				bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR);
				bgfx::setViewRect(viewId, 0, 0, 32, 32);
				bgfx::setViewTransform(viewId, captureViews[ii].Data(), captureProjection.Data());

				bgfx::setTexture(0, _uniforms.us_equirectangulaCubeMap, _uniforms._environmentViewTextureCube);
				Submit(cubeGeometry, viewId, NULL, _irradianceConvolution.GetProgram());
			}
		

			for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._prefilterTextureCubeFaceFb); ++ii)
			{
				unsigned int mip = ii / 6;
				unsigned int layer = ii % 6;
				unsigned int mipSize = 128 * std::pow(0.5, mip);

				bgfx::ViewId viewId = bgfx::ViewId(ii + 13);
				bgfx::setViewFrameBuffer(viewId, _uniforms._prefilterTextureCubeFaceFb[ii]);

				bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR);
				bgfx::setViewRect(viewId, 0, 0, mipSize, mipSize);
				bgfx::setViewTransform(viewId, captureViews[layer].Data(), captureProjection.Data());

				bgfx::setTexture(0, _uniforms.us_equirectangulaCubeMap, _uniforms._environmentViewTextureCube);
				float roughness = (float)mip / (float)(5 - 1);
				bgfx::setUniform(_uniforms.u_roughness, TVector4F(roughness,0.0f,0.0f,0.0f).Data());

				Submit(cubeGeometry, viewId, NULL, _prefilter.GetProgram());
			}

			for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._prefilterTextureCubeFaceFb); ++ii)
			{
				unsigned int mip = ii / 6;
				unsigned int layer = ii % 6;
				unsigned int mipSize = 128 * std::pow(0.5, mip);

				bgfx::blit(ii + 43, _uniforms._prefilterTextureCube, mip,
					0, 0, layer,
					_uniforms._prefilterTextureFbCube[mip], 0,
					0, 0, layer,
					mipSize, mipSize);
			}

		}
			
	}
	struct Uniforms
	{
		void Init()
		{
			us_equirectangularMap = bgfx::createUniform("s_equirectangularMap", bgfx::UniformType::Sampler);
			us_equirectangulaCubeMap = bgfx::createUniform("s_equirectangulaCubeMap", bgfx::UniformType::Sampler);
			u_roughness = bgfx::createUniform("u_roughness", bgfx::UniformType::Vec4);

			_environmentViewTextureCube = bgfx::createTextureCube(512, false, 1, bgfx::TextureFormat::RGBA16F, BGFX_TEXTURE_RT);
			for (uint32_t ii = 0; ii < BX_COUNTOF(_environmentViewTextureCubeFaceFb); ++ii)
			{
				bgfx::Attachment at;
				at.init(_environmentViewTextureCube, bgfx::Access::Write, uint16_t(ii));
				_environmentViewTextureCubeFaceFb[ii] = bgfx::createFrameBuffer(1, &at);
			}

			_irradianceViewTextureCube = bgfx::createTextureCube(32, false, 1, bgfx::TextureFormat::RGBA16F, BGFX_TEXTURE_RT);
			for (uint32_t ii = 0; ii < BX_COUNTOF(_environmentViewTextureCubeFaceFb); ++ii)
			{
				bgfx::Attachment at;
				at.init(_irradianceViewTextureCube, bgfx::Access::Write, uint16_t(ii));
				_irradianceViewTextureCubeFaceFb[ii] = bgfx::createFrameBuffer(1, &at);
			}


			_prefilterTextureCube = bgfx::createTextureCube(128, true, 1, bgfx::TextureFormat::RGBA16F, BGFX_TEXTURE_RT);
			unsigned int maxMipLevels = 5;
			for (int ii = 0; ii < maxMipLevels; ++ii)
			{
				unsigned int mip = ii;
				unsigned int mipSize = 128 * std::pow(0.5, mip);
				_prefilterTextureFbCube[ii] = bgfx::createTextureCube(mipSize, false, 1, bgfx::TextureFormat::RGBA16F, BGFX_TEXTURE_RT);
			}

			for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
			{
				for (uint32_t ii = 0; ii < 6; ++ii)
				{
					bgfx::Attachment at;
					at.init(_prefilterTextureFbCube[mip], bgfx::Access::Write, uint16_t(ii));
					_prefilterTextureCubeFaceFb[mip * 6 + ii] = bgfx::createFrameBuffer(1, &at);
				}
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
		bgfx::UniformHandle u_roughness;

		bgfx::TextureHandle _irradianceViewTextureCube;
		bgfx::FrameBufferHandle _irradianceViewTextureCubeFaceFb[6];

		bgfx::TextureHandle _prefilterTextureCube;
		bgfx::TextureHandle _prefilterTextureFbCube[5];
		bgfx::FrameBufferHandle _prefilterTextureCubeFaceFb[30];
	};
	
	Uniforms _uniforms;

	FShaderProgram _equirectangularToCubemap;
	FShaderProgram _irradianceConvolution;
	FShaderProgram _prefilter;

};

}