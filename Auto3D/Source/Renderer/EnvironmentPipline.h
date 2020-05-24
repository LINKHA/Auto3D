#pragma once
#include "AutoConfig.h"
#include "Component/CameraComponent.h"
#include "Component/SkyboxComponent.h"
#include "Renderer/IBLPipline.h"
#include "Math/Matrix4x4.h"
#include "Renderer/RendererDef.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{

class AUTO_API FEnvironmentPipline
{
public:
	FEnvironmentPipline() {}
	~FEnvironmentPipline() {}

	void Init();

	void Update(ACameraComponent* camera, ASkyboxComponent* skybox);

	void CreatePrefilteIrradiance(ACameraComponent* camera, ASkyboxComponent* skybox);

	struct Uniforms
	{
		void Init()
		{
			u_environmentViewMatrix = bgfx::createUniform("u_environmentViewMatrix", bgfx::UniformType::Mat4);
		
			us_texCube = bgfx::createUniform("s_texCube", bgfx::UniformType::Sampler);
			us_texCubeIrr = bgfx::createUniform("s_texCubeIrr", bgfx::UniformType::Sampler);



			//u_environmentViewMatrix = bgfx::createUniform("u_environmentViewMatrix", bgfx::UniformType::Mat4);
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

		void Submit()
		{
			bgfx::setUniform(u_environmentViewMatrix, _environmentViewMatrix.Data());
			bgfx::setTexture(0, us_texCube, _texture);
			bgfx::setTexture(1, us_texCubeIrr, _textureIrrance);
		}

		void Destroy()
		{
			bgfx::destroy(u_environmentViewMatrix);
		}

		TMatrix4x4F _environmentViewMatrix;
		bgfx::TextureHandle _texture;
		bgfx::TextureHandle _textureIrrance;

		bgfx::UniformHandle us_texCube;
		bgfx::UniformHandle us_texCubeIrr;
		bgfx::UniformHandle u_environmentViewMatrix;



		//TMatrix4x4F _environmentViewMatrix;
		//bgfx::UniformHandle u_environmentViewMatrix;

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