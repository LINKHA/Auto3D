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

		// View Transform 0.
		float view[16];
		bx::mtxIdentity(view);

		const bgfx::Caps* caps = bgfx::getCaps();

		float proj[16];
		bx::mtxOrtho(proj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0, caps->homogeneousDepth);
		bgfx::setViewTransform(RENDERVIEW_SKYBOX_ID, view, proj);
		bgfx::setViewRect(RENDERVIEW_SKYBOX_ID, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));


		TMatrix4x4F environmentViewMatrix = camera->GetEnvironmentViewMatrix();

		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		screenSpaceQuad((float)processWindow._width, (float)processWindow._height, true);

		_uniforms._environmentViewMatrix = environmentViewMatrix.Transpose();
		_uniforms._texture = skybox->GetHDRTexture()->GetTextureHandle();

		_uniforms.Submit();
		bgfx::submit(RENDERVIEW_SKYBOX_ID, _equirectangularToCubemap.GetProgram());
	}
	struct Uniforms
	{
		void Init()
		{
			u_environmentViewMatrix = bgfx::createUniform("u_environmentViewMatrix", bgfx::UniformType::Mat4);

			us_equirectangularMap = bgfx::createUniform("s_equirectangularMap", bgfx::UniformType::Sampler);
		}
		void Submit()
		{
			bgfx::setUniform(u_environmentViewMatrix, _environmentViewMatrix.Data());
			bgfx::setTexture(0, us_equirectangularMap, _texture);
		}

		void Destroy()
		{
			bgfx::destroy(u_environmentViewMatrix);
		}

		TMatrix4x4F _environmentViewMatrix;
		bgfx::TextureHandle _texture;

		bgfx::UniformHandle us_equirectangularMap;
		bgfx::UniformHandle u_environmentViewMatrix;

	};
	
	Uniforms _uniforms;

	FShaderProgram _equirectangularToCubemap;
	FShaderProgram _irradianceConvolution;
	FShaderProgram _prefilter;

};

}