#pragma once
#include "AutoConfig.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/RendererDef.h"
#include "Platform/ProcessWindow.h"
#include "Math/Matrix4x4.h"
#include "Component/CameraComponent.h"
#include "Component/SkyboxComponent.h"
#include "Renderer/RendererDef.h"
#include "Renderer/ViewPass.h"

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
		_background.AttachShader("vs_background", "fs_background");
		

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


		static bool rewrite = true;

		if (rewrite)
		{
			rewrite = false;

			for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._environmentViewTextureCubeFaceFb); ++ii)
			{
				//bgfx::ViewId viewId = bgfx::ViewId(ii + 1);
				bgfx::ViewId viewId = ViewIdConst();
				bgfx::setViewFrameBuffer(viewId, _uniforms._environmentViewTextureCubeFaceFb[ii]);
				bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR);
				bgfx::setViewRect(viewId, 0, 0, 512, 512);
				bgfx::setViewTransform(viewId, captureViews[ii].Data(), captureProjection.Data());

				bgfx::setTexture(0, _uniforms.us_equirectangularMap, _uniforms._environmentViewTexture);

				Submit(cubeGeometry, viewId, NULL, _equirectangularToCubemap.GetProgram());
			}
			bgfx::blit(ViewIdTemp(), _uniforms._environmentTempTexture[0], 0,
				0, 0, 0,
				_uniforms._environmentViewTextureCube, 0,
				0, 0, 3,
				512, 512);

			bgfx::blit(ViewIdTemp(), _uniforms._environmentTempTexture[1], 0,
				0, 0, 0,
				_uniforms._environmentViewTextureCube, 0,
				0, 0, 2,
				512, 512);

			bgfx::blit(ViewIdTemp(), _uniforms._environmentViewTextureCube, 0,
				0, 0, 3,
				_uniforms._environmentTempTexture[1], 0,
				0, 0, 0,
				512, 512);

			bgfx::blit(ViewIdTemp(), _uniforms._environmentViewTextureCube, 0,
				0, 0, 2,
				_uniforms._environmentTempTexture[0], 0,
				0, 0, 0,
				512, 512);

			/*for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._environmentViewTextureCubeFaceFb); ++ii)
			{
				bgfx::destroy(_uniforms._environmentViewTextureCubeFaceFb[ii]);
			}

*/
			
			/*bgfx::destroy(_uniforms._environmentTempTexture[0]);
			bgfx::destroy(_uniforms._environmentTempTexture[1]);*/


			for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._irradianceViewTextureCubeFaceFb); ++ii)
			{
				bgfx::ViewId viewId = ViewIdConst();
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

				bgfx::ViewId viewId = ViewIdConst();
				bgfx::setViewFrameBuffer(viewId, _uniforms._prefilterTextureCubeFaceFb[ii]);

				bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
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

				bgfx::blit(ViewIdTemp(), _uniforms._prefilterTextureCube, mip,
					0, 0, layer,
					_uniforms._prefilterTextureFbCube[mip], 0,
					0, 0, layer,
					mipSize, mipSize);
			}

		}
			
		if (!rewrite)
		{
			float view[16];
			bx::mtxIdentity(view);

			const bgfx::Caps* caps = bgfx::getCaps();
			bgfx::ViewId viewId = RENDERVIEW_SKYBOX_ID;
			//RENDERVIEW_SKYBOX_ID
			float proj[16];
			bx::mtxOrtho(proj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0, caps->homogeneousDepth);
			bgfx::setViewTransform(viewId, view, proj);
			bgfx::setViewFrameBuffer(viewId, BGFX_INVALID_HANDLE);
			bgfx::setViewRect(viewId, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
			screenSpaceQuad((float)processWindow._width, (float)processWindow._height, true);

			bgfx::setTexture(0, _uniforms.us_equirectangulaCubeMap, _uniforms._environmentViewTextureCube);
			bgfx::setUniform(_uniforms.u_environmentViewMatrix, _uniforms._environmentViewMatrix.Data());

			bgfx::submit(viewId, _background.GetProgram());
		}
		

	}
	struct Uniforms
	{
		void Init()
		{
			u_environmentViewMatrix = bgfx::createUniform("u_environmentViewMatrix", bgfx::UniformType::Mat4);
			us_equirectangularMap = bgfx::createUniform("s_equirectangularMap", bgfx::UniformType::Sampler);
			us_equirectangulaCubeMap = bgfx::createUniform("s_equirectangulaCubeMap", bgfx::UniformType::Sampler);
			u_roughness = bgfx::createUniform("u_roughness", bgfx::UniformType::Vec4);

			_environmentTempTexture[0] = bgfx::createTexture2D(512, 512, false, 1, bgfx::TextureFormat::RGBA16F, BGFX_TEXTURE_RT);
			_environmentTempTexture[1] = bgfx::createTexture2D(512, 512, false, 1, bgfx::TextureFormat::RGBA16F, BGFX_TEXTURE_RT);

			_environmentViewTextureCube = bgfx::createTextureCube(512, false, 1, bgfx::TextureFormat::RGBA16F, BGFX_TEXTURE_RT);
			for (uint32_t ii = 0; ii < BX_COUNTOF(_environmentViewTextureCubeFaceFb); ++ii)
			{
				bgfx::Attachment at;
				at.init(_environmentViewTextureCube, bgfx::Access::Write, uint16_t(ii));
				_environmentViewTextureCubeFaceFb[ii] = bgfx::createFrameBuffer(1, &at, false);
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
		bgfx::UniformHandle u_environmentViewMatrix;

		bgfx::TextureHandle _environmentViewTexture;

		bgfx::TextureHandle _environmentTempTexture[2];
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
	FShaderProgram _background;

};

}