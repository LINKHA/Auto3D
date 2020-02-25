#include "Renderer/ShadowRenderer.h"

namespace Auto3D
{
IMPLEMENT_SINGLETON(FShadowRenderer)

Programs FShadowRenderer::s_programs;
Uniforms FShadowRenderer::s_uniforms;

bool FShadowRenderer::s_flipV = false;
float FShadowRenderer::s_texelHalf = 0.0f;
bgfx::UniformHandle FShadowRenderer::s_texColor;
bgfx::UniformHandle FShadowRenderer::s_shadowMap[ShadowMapRenderTargets::Count];

FShadowRenderer::FShadowRenderer() 
{}
FShadowRenderer::~FShadowRenderer() 
{}
void FShadowRenderer::init()
{
	// Setup root path for binary shaders. Shader binaries are different
	// for each renderer.
	switch (bgfx::getRendererType())
	{
	case bgfx::RendererType::Direct3D9:
		s_texelHalf = 0.5f;
		break;

	case bgfx::RendererType::OpenGL:
	case bgfx::RendererType::OpenGLES:
		s_flipV = true;
		break;

	default:
		break;
	}

	// Uniforms.
	FShadowRenderer::s_uniforms.init();

	FShadowRenderer::s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
	FShadowRenderer::s_shadowMap[0] = bgfx::createUniform("s_shadowMap0", bgfx::UniformType::Sampler);
	FShadowRenderer::s_shadowMap[1] = bgfx::createUniform("s_shadowMap1", bgfx::UniformType::Sampler);
	FShadowRenderer::s_shadowMap[2] = bgfx::createUniform("s_shadowMap2", bgfx::UniformType::Sampler);
	FShadowRenderer::s_shadowMap[3] = bgfx::createUniform("s_shadowMap3", bgfx::UniformType::Sampler);

	// Programs.
	FShadowRenderer::s_programs.init();
}
void FShadowRenderer::update()
{

}

}