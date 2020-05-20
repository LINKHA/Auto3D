#pragma once
#include "AutoConfig.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/RendererDef.h"
#include "Platform/ProcessWindow.h"
#include "Math/Matrix4x4.h"
#include "Component/CameraComponent.h"
#include "Component/SkyboxComponent.h"

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
		/*_irradianceConvolution.AttachShader("vs_cube_map", "fs_irradiance_convolution");
		_prefilter.AttachShader("vs_cube_map", "fs_prefilter");*/

		

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

			_uniforms.Submit();

			//Set the six-sided camera to read cubemap
			bgfx::submit(viewId, _equirectangularToCubemap.GetProgram());
			//bgfx::touch(viewId);
		}

	}
	struct Uniforms
	{
		void Init()
		{
			u_environmentViewMatrix = bgfx::createUniform("u_environmentViewMatrix", bgfx::UniformType::Mat4);

			us_equirectangularMap = bgfx::createUniform("s_equirectangularMap", bgfx::UniformType::Sampler);


			_environmentViewTextureCube = bgfx::createTextureCube(512, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT);
			for (uint32_t ii = 0; ii < BX_COUNTOF(_environmentViewTextureCubeFaceFb); ++ii)
			{
				bgfx::Attachment at;
				at.init(_environmentViewTextureCube, bgfx::Access::Write, uint16_t(ii));
				_environmentViewTextureCubeFaceFb[ii] = bgfx::createFrameBuffer(1, &at);
			}
		}

		void Update()
		{
		}

		void Submit()
		{
			bgfx::setUniform(u_environmentViewMatrix, _environmentViewMatrix.Data());
			bgfx::setTexture(0, us_equirectangularMap, _environmentViewTexture);
		}

		void Destroy()
		{
			bgfx::destroy(u_environmentViewMatrix);
		}

		TMatrix4x4F _environmentViewMatrix;
		bgfx::TextureHandle _environmentViewTexture;

		bgfx::TextureHandle _environmentViewTextureCube;

		bgfx::FrameBufferHandle _environmentViewTextureCubeFaceFb[6];

		bgfx::UniformHandle us_equirectangularMap;
		bgfx::UniformHandle u_environmentViewMatrix;

	};
	
	Uniforms _uniforms;

	FShaderProgram _equirectangularToCubemap;
	FShaderProgram _irradianceConvolution;
	FShaderProgram _prefilter;

};

}