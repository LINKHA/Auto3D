#pragma once
#include "AutoConfig.h"
#include "Component/CameraComponent.h"
#include "Component/SkyboxComponent.h"
#include "Renderer/IBLRenderer.h"
#include "Math/Matrix4x4.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{

class AUTO_API FEnvironmentRenderer
{
public:
	FEnvironmentRenderer() {}
	~FEnvironmentRenderer() {}
	void Init()
	{
		_uniforms.Init();
	}
	void Update(ACameraComponent* camera,ASkyboxComponent* skybox)
	{
		if (camera == nullptr || skybox == nullptr)
			return;

		GProcessWindow& processWindow = GProcessWindow::Get();

		// View Transform 0.
		float view[16];
		bx::mtxIdentity(view);

		const bgfx::Caps* caps = bgfx::getCaps();

		float proj[16];
		bx::mtxOrtho(proj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0, caps->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);
		bgfx::setViewRect(0, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));
		
		
		TMatrix4x4F environmentViewMatrix = camera->GetEnvironmentViewMatrix();

		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		screenSpaceQuad((float)processWindow._width, (float)processWindow._height, true);

		_uniforms._environmentViewMatrix = environmentViewMatrix.Transpose();
		_uniforms._texture = skybox->GetTexture()->GetTextureHandle();
		_uniforms._textureIrrance = skybox->GetIrranceTexture()->GetTextureHandle();

		_uniforms.Submit();
		bgfx::submit(0, skybox->GetShaderProgram().GetProgram());
	}
	struct Uniforms
	{
		void Init()
		{
			u_environmentViewMatrix = bgfx::createUniform("u_environmentViewMatrix", bgfx::UniformType::Mat4);
		
			s_texCube = bgfx::createUniform("s_texCube", bgfx::UniformType::Sampler);
			s_texCubeIrr = bgfx::createUniform("s_texCubeIrr", bgfx::UniformType::Sampler);
		}

		void Submit()
		{
			bgfx::setUniform(u_environmentViewMatrix, _environmentViewMatrix.Data());
			bgfx::setTexture(0, s_texCube, _texture);
			bgfx::setTexture(1, s_texCubeIrr, _textureIrrance);
		}

		void Destroy()
		{
			bgfx::destroy(u_environmentViewMatrix);
		}

		TMatrix4x4F _environmentViewMatrix;
		bgfx::TextureHandle _texture;
		bgfx::TextureHandle _textureIrrance;

		bgfx::UniformHandle s_texCube;
		bgfx::UniformHandle s_texCubeIrr;
		bgfx::UniformHandle u_environmentViewMatrix;
	};
	Uniforms _uniforms;
};

}