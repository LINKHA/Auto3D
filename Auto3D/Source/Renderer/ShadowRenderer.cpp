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
ShadowMapSettings FShadowRenderer::s_smSettings[LightType::Count][DepthImpl::Count][SmImpl::Count];
SceneSettings FShadowRenderer::s_settings;
uint16_t FShadowRenderer::s_currentShadowMapSize;
bgfx::FrameBufferHandle FShadowRenderer::s_rtShadowMap[ShadowMapRenderTargets::Count];
bgfx::FrameBufferHandle FShadowRenderer::s_rtBlur;

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

	ShadowMapSettings smSettings[LightType::Count][DepthImpl::Count][SmImpl::Count]=
	{
		{ //LightType::Spot

			{ //DepthImpl::InvZ

				{ //SmImpl::Hard
					10.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 10.0f, 1.0f, 20.0f, 1.0f         // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 1.0f          // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.0035f, 0.0f, 0.01f, 0.00001f   // m_bias
					, 0.0012f, 0.0f, 0.05f, 0.00001f   // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 500.0f, 1.0f, 1000.0f, 1.0f      // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_xOffset
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Single][DepthImpl::InvZ][SmImpl::Hard] //m_progDraw
				},
				{ //SmImpl::PCF
					10.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 10.0f, 1.0f, 20.0f, 1.0f         // m_depthValuePow
					, 1.0f, 1.0f, 99.0f, 1.0f          // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.007f, 0.0f, 0.01f, 0.00001f    // m_bias
					, 0.001f, 0.0f, 0.05f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 500.0f, 1.0f, 1000.0f, 1.0f      // m_customParam1
					, 2.0f, 0.0f, 8.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 8.0f, 1.0f           // m_yNum
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_xOffset
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Single][DepthImpl::InvZ][SmImpl::PCF] //m_progDraw
				},
				{ //SmImpl::VSM
					10.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 10.0f, 1.0f, 20.0f, 1.0f         // m_depthValuePow
					, 8.0f, 1.0f, 10.0f, 1.0f          // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.045f, 0.0f, 0.1f, 0.00001f     // m_bias
					, 0.001f, 0.0f, 0.05f, 0.00001f    // m_normalOffset
					, 0.02f, 0.0f, 0.04f, 0.00001f     // m_customParam0
					, 450.0f, 1.0f, 1000.0f, 1.0f      // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_xOffset
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::InvZ][PackDepth::VSM] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Single][DepthImpl::InvZ][SmImpl::VSM] //m_progDraw
				},
				{ //SmImpl::ESM
					10.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 10.0f, 1.0f, 20.0f, 1.0f         // m_depthValuePow
					, 3.0f, 1.0f, 10.0f, 0.01f         // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.02f, 0.0f, 0.3f, 0.00001f      // m_bias
					, 0.001f, 0.0f, 0.05f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 9000.0f, 1.0f, 15000.0f, 1.0f    // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_xOffset
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Single][DepthImpl::InvZ][SmImpl::ESM] //m_progDraw
				}

			},
			{ //DepthImpl::Linear

				{ //SmImpl::Hard
					10.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 1.0f          // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.0025f, 0.0f, 0.01f, 0.00001f   // m_bias
					, 0.0012f, 0.0f, 0.05f, 0.00001f   // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 500.0f, 1.0f, 1000.0f, 1.0f      // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_xOffset
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Single][DepthImpl::Linear][SmImpl::Hard] //m_progDraw
				},
				{ //SmImpl::PCF
					10.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 99.0f, 1.0f          // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.0025f, 0.0f, 0.01f, 0.00001f   // m_bias
					, 0.001f, 0.0f, 0.05f,  0.00001f   // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 2000.0f, 1.0f, 2000.0f, 1.0f     // m_customParam1
					, 2.0f, 0.0f, 8.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 8.0f, 1.0f           // m_yNum
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_xOffset
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Single][DepthImpl::Linear][SmImpl::PCF] //m_progDraw
				},
				{ //SmImpl::VSM
					10.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 1.0f          // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.006f, 0.0f, 0.01f, 0.00001f    // m_bias
					, 0.001f, 0.0f, 0.05f, 0.00001f    // m_normalOffset
					, 0.02f, 0.0f, 0.1f, 0.00001f      // m_customParam0
					, 300.0f, 1.0f, 1500.0f, 1.0f      // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_xOffset
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::Linear][PackDepth::VSM] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Single][DepthImpl::Linear][SmImpl::VSM] //m_progDraw
				},
				{ //SmImpl::ESM
					10.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 0.01f         // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.0055f, 0.0f, 0.01f, 0.00001f   // m_bias
					, 0.001f, 0.0f, 0.05f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 2500.0f, 1.0f, 5000.0f, 1.0f     // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_xOffset
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Single][DepthImpl::Linear][SmImpl::ESM] //m_progDraw
				}

			}

		},
		{ //LightType::Point

			{ //DepthImpl::InvZ

				{ //SmImpl::Hard
					12.0f, 9.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 10.0f, 1.0f, 20.0f, 1.0f         // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 1.0f          // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.006f, 0.0f, 0.01f, 0.00001f    // m_bias
					, 0.001f, 0.0f, 0.05f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 50.0f, 1.0f, 300.0f, 1.0f        // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 0.25f, 0.0f, 2.0f, 0.001f        // m_xOffset
					, 0.25f, 0.0f, 2.0f, 0.001f        // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Omni][DepthImpl::InvZ][SmImpl::Hard] //m_progDraw
				},
				{ //SmImpl::PCF
					12.0f, 9.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 10.0f, 1.0f, 20.0f, 1.0f         // m_depthValuePow
					, 1.0f, 1.0f, 99.0f, 1.0f          // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.004f, 0.0f, 0.01f, 0.00001f    // m_bias
					, 0.001f, 0.0f, 0.05f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 50.0f, 1.0f, 300.0f, 1.0f        // m_customParam1
					, 2.0f, 0.0f, 8.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 8.0f, 1.0f           // m_yNum
					, 1.0f, 0.0f, 3.0f, 0.001f         // m_xOffset
					, 1.0f, 0.0f, 3.0f, 0.001f         // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Omni][DepthImpl::InvZ][SmImpl::PCF] //m_progDraw
				},
				{ //SmImpl::VSM
					12.0f, 9.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 10.0f, 1.0f, 20.0f, 1.0f         // m_depthValuePow
					, 8.0f, 1.0f, 10.0f, 1.0f          // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.055f, 0.0f, 0.1f, 0.00001f     // m_bias
					, 0.001f, 0.0f, 0.05f, 0.00001f    // m_normalOffset
					, 0.02f, 0.0f, 0.04f, 0.00001f     // m_customParam0
					, 450.0f, 1.0f, 900.0f, 1.0f       // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 0.25f, 0.0f, 2.0f, 0.001f        // m_xOffset
					, 0.25f, 0.0f, 2.0f, 0.001f        // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::InvZ][PackDepth::VSM] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Omni][DepthImpl::InvZ][SmImpl::VSM] //m_progDraw
				},
				{ //SmImpl::ESM
					12.0f, 9.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 10.0f, 1.0f, 20.0f, 1.0f         // m_depthValuePow
					, 3.0f, 1.0f, 10.0f, 0.01f         // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.035f, 0.0f, 0.1f, 0.00001f     // m_bias
					, 0.001f, 0.0f, 0.05f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 9000.0f, 1.0f, 15000.0f, 1.0f    // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 0.25f, 0.0f, 2.0f, 0.001f        // m_xOffset
					, 0.25f, 0.0f, 2.0f, 0.001f        // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Omni][DepthImpl::InvZ][SmImpl::ESM] //m_progDraw
				}

			},
			{ //DepthImpl::Linear

				{ //SmImpl::Hard
					12.0f, 9.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 1.0f          // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.003f, 0.0f, 0.01f, 0.00001f    // m_bias
					, 0.001f, 0.0f, 0.05f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 120.0f, 1.0f, 300.0f, 1.0f       // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 0.25f, 0.0f, 2.0f, 0.001f        // m_xOffset
					, 0.25f, 0.0f, 2.0f, 0.001f        // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Omni][DepthImpl::Linear][SmImpl::Hard] //m_progDraw
				},
				{ //SmImpl::PCF
					12.0f, 9.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 99.0f, 1.0f          // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.0035f, 0.0f, 0.01f, 0.00001f   // m_bias
					, 0.001f, 0.0f, 0.05f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 120.0f, 1.0f, 300.0f, 1.0f       // m_customParam1
					, 2.0f, 0.0f, 8.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 8.0f, 1.0f           // m_yNum
					, 1.0f, 0.0f, 3.0f, 0.001f         // m_xOffset
					, 1.0f, 0.0f, 3.0f, 0.001f         // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Omni][DepthImpl::Linear][SmImpl::PCF] //m_progDraw
				},
				{ //SmImpl::VSM
					12.0f, 9.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 1.0f          // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.006f, 0.0f, 0.1f, 0.00001f     // m_bias
					, 0.001f, 0.0f, 0.05f, 0.00001f    // m_normalOffset
					, 0.02f, 0.0f, 0.1f, 0.00001f      // m_customParam0
					, 400.0f, 1.0f, 900.0f, 1.0f       // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 0.25f, 0.0f, 2.0f, 0.001f        // m_xOffset
					, 0.25f, 0.0f, 2.0f, 0.001f        // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::Linear][PackDepth::VSM] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Omni][DepthImpl::Linear][SmImpl::VSM] //m_progDraw
				},
				{ //SmImpl::ESM
					12.0f, 9.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 0.01f         // m_near
					, 250.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.007f, 0.0f, 0.01f, 0.00001f    // m_bias
					, 0.001f, 0.0f, 0.05f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 8000.0f, 1.0f, 15000.0f, 1.0f    // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 0.25f, 0.0f, 2.0f, 0.001f        // m_xOffset
					, 0.25f, 0.0f, 2.0f, 0.001f        // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Omni][DepthImpl::Linear][SmImpl::ESM] //m_progDraw
				}

			}

		},
		{ //LightType::Directional

			{ //DepthImpl::InvZ

				{ //SmImpl::Hard
					11.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 1.0f          // m_near
					, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.0012f, 0.0f, 0.01f, 0.00001f   // m_bias
					, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 200.0f, 1.0f, 400.0f, 1.0f       // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 0.2f, 0.0f, 1.0f, 0.01f          // m_xOffset
					, 0.2f, 0.0f, 1.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Cascade][DepthImpl::InvZ][SmImpl::Hard] //m_progDraw
				},
				{ //SmImpl::PCF
					11.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 99.0f, 1.0f          // m_near
					, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.0012f, 0.0f, 0.01f, 0.00001f   // m_bias
					, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 200.0f, 1.0f, 400.0f, 1.0f       // m_customParam1
					, 2.0f, 0.0f, 8.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 8.0f, 1.0f           // m_yNum
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_xOffset
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Cascade][DepthImpl::InvZ][SmImpl::PCF] //m_progDraw
				},
				{ //SmImpl::VSM
					11.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 1.0f          // m_near
					, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.004f, 0.0f, 0.01f, 0.00001f    // m_bias
					, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
					, 0.02f, 0.0f, 0.04f, 0.00001f     // m_customParam0
					, 2500.0f, 1.0f, 5000.0f, 1.0f     // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 0.2f, 0.0f, 1.0f, 0.01f          // m_xOffset
					, 0.2f, 0.0f, 1.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::InvZ][PackDepth::VSM] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Cascade][DepthImpl::InvZ][SmImpl::VSM] //m_progDraw
				},
				{ //SmImpl::ESM
					11.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 0.01f         // m_near
					, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.004f, 0.0f, 0.01f, 0.00001f    // m_bias
					, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 9500.0f, 1.0f, 15000.0f, 1.0f    // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 0.2f, 0.0f, 1.0f, 0.01f          // m_xOffset
					, 0.2f, 0.0f, 1.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Cascade][DepthImpl::InvZ][SmImpl::ESM] //m_progDraw
				}

			},
			{ //DepthImpl::Linear

				{ //SmImpl::Hard
					11.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 1.0f          // m_near
					, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.0012f, 0.0f, 0.01f, 0.00001f   // m_bias
					, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 500.0f, 1.0f, 1000.0f, 1.0f      // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 0.2f, 0.0f, 1.0f, 0.01f          // m_xOffset
					, 0.2f, 0.0f, 1.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Cascade][DepthImpl::Linear][SmImpl::Hard] //m_progDraw
				},
				{ //SmImpl::PCF
					11.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 99.0f, 1.0f          // m_near
					, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.0012f, 0.0f, 0.01f, 0.00001f   // m_bias
					, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 200.0f, 1.0f, 400.0f, 1.0f       // m_customParam1
					, 2.0f, 0.0f, 8.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 8.0f, 1.0f           // m_yNum
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_xOffset
					, 1.0f, 0.0f, 3.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Cascade][DepthImpl::Linear][SmImpl::PCF] //m_progDraw
				},
				{ //SmImpl::VSM
					11.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 1.0f          // m_near
					, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.004f, 0.0f, 0.01f, 0.00001f    // m_bias
					, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
					, 0.02f, 0.0f, 0.04f, 0.00001f     // m_customParam0
					, 2500.0f, 1.0f, 5000.0f, 1.0f     // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 0.2f, 0.0f, 1.0f, 0.01f          // m_xOffset
					, 0.2f, 0.0f, 1.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::Linear][PackDepth::VSM] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Cascade][DepthImpl::Linear][SmImpl::VSM] //m_progDraw
				},
				{ //SmImpl::ESM
					11.0f, 7.0f, 12.0f, 1.0f         // m_sizePwrTwo
					, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
					, 1.0f, 1.0f, 10.0f, 0.01f         // m_near
					, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
					, 0.004f, 0.0f, 0.01f, 0.00001f    // m_bias
					, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
					, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
					, 9500.0f, 1.0f, 15000.0f, 1.0f    // m_customParam1
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
					, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
					, 0.2f, 0.0f, 1.0f, 0.01f          // m_xOffset
					, 0.2f, 0.0f, 1.0f, 0.01f          // m_yOffset
					, true                             // m_doBlur
					, &FShadowRenderer::s_programs.m_packDepth[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
					, &FShadowRenderer::s_programs.m_colorLighting[SmType::Cascade][DepthImpl::Linear][SmImpl::ESM] //m_progDraw
				}

			}
		}
	};
	bx::memCopy(s_smSettings, smSettings, sizeof(smSettings));

	FShadowRenderer::s_settings.m_lightType = LightType::SpotLight;
	FShadowRenderer::s_settings.m_depthImpl = DepthImpl::InvZ;
	FShadowRenderer::s_settings.m_smImpl = SmImpl::Hard;
	FShadowRenderer::s_settings.m_spotOuterAngle = 45.0f;
	FShadowRenderer::s_settings.m_spotInnerAngle = 30.0f;
	FShadowRenderer::s_settings.m_fovXAdjust = 0.0f;
	FShadowRenderer::s_settings.m_fovYAdjust = 0.0f;
	FShadowRenderer::s_settings.m_coverageSpotL = 90.0f;
	FShadowRenderer::s_settings.m_splitDistribution = 0.6f;
	FShadowRenderer::s_settings.m_numSplits = 4;
	FShadowRenderer::s_settings.m_updateLights = true;
	FShadowRenderer::s_settings.m_updateScene = true;
	FShadowRenderer::s_settings.m_drawDepthBuffer = false;
	FShadowRenderer::s_settings.m_showSmCoverage = false;
	FShadowRenderer::s_settings.m_stencilPack = true;
	FShadowRenderer::s_settings.m_stabilize = true;

	ShadowMapSettings* currentSmSettings = &FShadowRenderer::s_smSettings[FShadowRenderer::s_settings.m_lightType][FShadowRenderer::s_settings.m_depthImpl][FShadowRenderer::s_settings.m_smImpl];

	// Render targets.
	uint16_t shadowMapSize = 1 << uint32_t(currentSmSettings->m_sizePwrTwo);
	s_currentShadowMapSize = shadowMapSize;
	float currentShadowMapSizef = float(int16_t(s_currentShadowMapSize));
	FShadowRenderer::s_uniforms.m_shadowMapTexelSize = 1.0f / currentShadowMapSizef;

	for (uint8_t ii = 0; ii < ShadowMapRenderTargets::Count; ++ii)
	{
		bgfx::TextureHandle fbtextures[] =
		{
			bgfx::createTexture2D(FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT),
			bgfx::createTexture2D(FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize, false, 1, bgfx::TextureFormat::D24S8, BGFX_TEXTURE_RT),
		};
		FShadowRenderer::s_rtShadowMap[ii] = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
	}
	FShadowRenderer::s_rtBlur = bgfx::createFrameBuffer(FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize, bgfx::TextureFormat::BGRA8);

}
void FShadowRenderer::update()
{

}

}