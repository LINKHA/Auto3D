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

	struct Uniforms
	{
		void Init()
		{
			u_environmentViewMatrix = bgfx::createUniform("u_environmentViewMatrix", bgfx::UniformType::Mat4);
		
			us_texCube = bgfx::createUniform("s_texCube", bgfx::UniformType::Sampler);
			us_texCubeIrr = bgfx::createUniform("s_texCubeIrr", bgfx::UniformType::Sampler);
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
	};
	Uniforms _uniforms;
};

}