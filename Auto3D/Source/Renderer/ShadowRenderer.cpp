#include "Renderer/ShadowRenderer.h"
#include "Platform/ProcessWindow.h"

#include <bx/timer.h>

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

Material FShadowRenderer::s_defaultMaterial;
Light FShadowRenderer::s_pointLight;
Light FShadowRenderer::s_directionalLight;
float FShadowRenderer::s_color[4];
float FShadowRenderer::s_lightMtx[16];
float FShadowRenderer::s_shadowMapMtx[ShadowMapRenderTargets::Count][16];

ViewState FShadowRenderer::s_viewState;
ClearValues FShadowRenderer::s_clearValues;

ACameraComponent* FShadowRenderer::s_camera;

float FShadowRenderer::s_timeAccumulatorLight;
float FShadowRenderer::s_timeAccumulatorScene;

FShadowRenderer::FShadowRenderer() 
{}
FShadowRenderer::~FShadowRenderer() 
{}
void FShadowRenderer::init()
{
	GProcessWindow& processWindow = GProcessWindow::Get();

	FShadowRenderer::s_viewState = ViewState(uint16_t(processWindow._width), uint16_t(processWindow._height));
	FShadowRenderer::s_clearValues = ClearValues(0x00000000, 1.0f, 0);


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

	FShadowRenderer::s_uniforms.submitConstUniforms();

	// Materials.
	s_defaultMaterial =
	{
		{ { 1.0f, 1.0f, 1.0f, 0.0f } }, //ambient
		{ { 1.0f, 1.0f, 1.0f, 0.0f } }, //diffuse
		{ { 1.0f, 1.0f, 1.0f, 0.0f } }, //specular, exponent
	};

	// Lights.
	s_pointLight =
	{
		{ { 0.0f, 0.0f, 0.0f, 1.0f   } }, //position
		{   0.0f, 0.0f, 0.0f, 0.0f     }, //-ignore
		{ { 1.0f, 1.0f, 1.0f, 0.0f   } }, //ambient
		{ { 1.0f, 1.0f, 1.0f, 850.0f } }, //diffuse
		{ { 1.0f, 1.0f, 1.0f, 0.0f   } }, //specular
		{ { 0.0f,-0.4f,-0.6f, 0.0f   } }, //spotdirection, spotexponent
		{   0.0f, 0.0f, 0.0f, 0.0f     }, //-ignore
		{ { 1.0f, 0.0f, 1.0f, 91.0f  } }, //attenuation, spotcutoff
	};

	s_directionalLight =
	{
		{ { 0.5f,-1.0f, 0.1f, 0.0f  } }, //position
		{   0.0f, 0.0f, 0.0f, 0.0f    }, //-ignore
		{ { 1.0f, 1.0f, 1.0f, 0.02f } }, //ambient
		{ { 1.0f, 1.0f, 1.0f, 0.4f  } }, //diffuse
		{ { 1.0f, 1.0f, 1.0f, 0.0f  } }, //specular
		{ { 0.0f, 0.0f, 0.0f, 1.0f  } }, //spotdirection, spotexponent
		{   0.0f, 0.0f, 0.0f, 0.0f    }, //-ignore
		{ { 0.0f, 0.0f, 0.0f, 1.0f  } }, //attenuation, spotcutoff
	};

	// Setup uniforms.
	FShadowRenderer::s_color[0] = FShadowRenderer::s_color[1] = FShadowRenderer::s_color[2] = FShadowRenderer::s_color[3] = 1.0f;
	FShadowRenderer::s_uniforms.setPtrs(&FShadowRenderer::s_defaultMaterial
		, &FShadowRenderer::s_pointLight
		, FShadowRenderer::s_color
		, FShadowRenderer::s_lightMtx
		, &FShadowRenderer::s_shadowMapMtx[ShadowMapRenderTargets::First][0]
		, &FShadowRenderer::s_shadowMapMtx[ShadowMapRenderTargets::Second][0]
		, &FShadowRenderer::s_shadowMapMtx[ShadowMapRenderTargets::Third][0]
		, &FShadowRenderer::s_shadowMapMtx[ShadowMapRenderTargets::Fourth][0]
	);

	FShadowRenderer::s_timeAccumulatorLight = 0.0f;
	FShadowRenderer::s_timeAccumulatorScene = 0.0f;
}
void FShadowRenderer::update()
{
	GProcessWindow& processWindow = GProcessWindow::Get();

	FShadowRenderer::s_viewState.m_width = uint16_t(processWindow._width);
	FShadowRenderer::s_viewState.m_height = uint16_t(processWindow._height);

	const bgfx::Caps* caps = bgfx::getCaps();

	// Set view and projection matrices.
	const float camFovy = s_camera->GetFov();
	const float camAspect = s_camera->GetAspectRatio();
	const float projHeight = bx::tan(bx::toRad(camFovy)*0.5f);
	const float projWidth = projHeight * camAspect;

	s_camera->SetAspectRatio(float(GProcessWindow::Get()._width) / float(GProcessWindow::Get()._height));

	TMatrix4x4F projectionMatrix = s_camera->GetProjectionMatrix();
	FShadowRenderer::s_viewState.m_proj[0] = projectionMatrix._m00;
	FShadowRenderer::s_viewState.m_proj[1] = projectionMatrix._m01;
	FShadowRenderer::s_viewState.m_proj[2] = projectionMatrix._m02;
	FShadowRenderer::s_viewState.m_proj[3] = projectionMatrix._m03;
	FShadowRenderer::s_viewState.m_proj[4] = projectionMatrix._m10;
	FShadowRenderer::s_viewState.m_proj[5] = projectionMatrix._m11;
	FShadowRenderer::s_viewState.m_proj[6] = projectionMatrix._m12;
	FShadowRenderer::s_viewState.m_proj[7] = projectionMatrix._m13;
	FShadowRenderer::s_viewState.m_proj[8] = projectionMatrix._m20;
	FShadowRenderer::s_viewState.m_proj[9] = projectionMatrix._m21;
	FShadowRenderer::s_viewState.m_proj[10] = projectionMatrix._m22;
	FShadowRenderer::s_viewState.m_proj[11] = projectionMatrix._m23;
	FShadowRenderer::s_viewState.m_proj[12] = projectionMatrix._m30;
	FShadowRenderer::s_viewState.m_proj[13] = projectionMatrix._m31;
	FShadowRenderer::s_viewState.m_proj[14] = projectionMatrix._m32;
	FShadowRenderer::s_viewState.m_proj[15] = projectionMatrix._m33;

	TMatrix3x4F viewMatrix = s_camera->GetViewMatrix();
	TMatrix4x4F transposeViewMatrix = viewMatrix.ToMatrix4().Transpose();
	FShadowRenderer::s_viewState.m_view[0] = transposeViewMatrix._m00;
	FShadowRenderer::s_viewState.m_view[1] = transposeViewMatrix._m01;
	FShadowRenderer::s_viewState.m_view[2] = transposeViewMatrix._m02;
	FShadowRenderer::s_viewState.m_view[3] = transposeViewMatrix._m03;
	FShadowRenderer::s_viewState.m_view[4] = transposeViewMatrix._m10;
	FShadowRenderer::s_viewState.m_view[5] = transposeViewMatrix._m11;
	FShadowRenderer::s_viewState.m_view[6] = transposeViewMatrix._m12;
	FShadowRenderer::s_viewState.m_view[7] = transposeViewMatrix._m13;
	FShadowRenderer::s_viewState.m_view[8] = transposeViewMatrix._m20;
	FShadowRenderer::s_viewState.m_view[9] = transposeViewMatrix._m21;
	FShadowRenderer::s_viewState.m_view[10] = transposeViewMatrix._m22;
	FShadowRenderer::s_viewState.m_view[11] = transposeViewMatrix._m23;
	FShadowRenderer::s_viewState.m_view[12] = transposeViewMatrix._m30;
	FShadowRenderer::s_viewState.m_view[13] = transposeViewMatrix._m31;
	FShadowRenderer::s_viewState.m_view[14] = transposeViewMatrix._m32;
	FShadowRenderer::s_viewState.m_view[15] = transposeViewMatrix._m33;

	float currentShadowMapSizef = float(int16_t(FShadowRenderer::s_currentShadowMapSize));
	FShadowRenderer::s_uniforms.m_shadowMapTexelSize = 1.0f / currentShadowMapSizef;

	FShadowRenderer::s_uniforms.submitConstUniforms();

	ShadowMapSettings* currentShadowMapSettings = &FShadowRenderer::s_smSettings[FShadowRenderer::s_settings.m_lightType][FShadowRenderer::s_settings.m_depthImpl][FShadowRenderer::s_settings.m_smImpl];
	// Update uniforms.
	FShadowRenderer::s_uniforms.m_shadowMapBias = currentShadowMapSettings->m_bias;
	FShadowRenderer::s_uniforms.m_shadowMapOffset = currentShadowMapSettings->m_normalOffset;
	FShadowRenderer::s_uniforms.m_shadowMapParam0 = currentShadowMapSettings->m_customParam0;
	FShadowRenderer::s_uniforms.m_shadowMapParam1 = currentShadowMapSettings->m_customParam1;
	FShadowRenderer::s_uniforms.m_depthValuePow = currentShadowMapSettings->m_depthValuePow;
	FShadowRenderer::s_uniforms.m_XNum = currentShadowMapSettings->m_xNum;
	FShadowRenderer::s_uniforms.m_YNum = currentShadowMapSettings->m_yNum;
	FShadowRenderer::s_uniforms.m_XOffset = currentShadowMapSettings->m_xOffset;
	FShadowRenderer::s_uniforms.m_YOffset = currentShadowMapSettings->m_yOffset;
	FShadowRenderer::s_uniforms.m_showSmCoverage = float(FShadowRenderer::s_settings.m_showSmCoverage);
	FShadowRenderer::s_uniforms.m_lightPtr = (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType) ? &FShadowRenderer::s_directionalLight : &FShadowRenderer::s_pointLight;

	if (LightType::SpotLight == FShadowRenderer::s_settings.m_lightType)
	{
		FShadowRenderer::s_pointLight.m_attenuationSpotOuter.m_outer = FShadowRenderer::s_settings.m_spotOuterAngle;
		FShadowRenderer::s_pointLight.m_spotDirectionInner.m_inner = FShadowRenderer::s_settings.m_spotInnerAngle;
	}
	else
	{
		FShadowRenderer::s_pointLight.m_attenuationSpotOuter.m_outer = 91.0f; //above 90.0f means point light
	}

	FShadowRenderer::s_uniforms.submitPerFrameUniforms();

	int64_t now = bx::getHPCounter();
	static int64_t last = now;
	const int64_t frameTime = now - last;
	last = now;
	const double freq = double(bx::getHPFrequency());
	const float deltaTime = float(frameTime / freq);

	// Update lights.
	FShadowRenderer::s_pointLight.computeViewSpaceComponents(FShadowRenderer::s_viewState.m_view);
	FShadowRenderer::s_directionalLight.computeViewSpaceComponents(FShadowRenderer::s_viewState.m_view);

	// Update time accumulators.
	if (FShadowRenderer::s_settings.m_updateLights) { FShadowRenderer::s_timeAccumulatorLight += deltaTime; }
	if (FShadowRenderer::s_settings.m_updateScene) { FShadowRenderer::s_timeAccumulatorScene += deltaTime; }

	// Setup lights.
	FShadowRenderer::s_pointLight.m_position.m_x = bx::cos(FShadowRenderer::s_timeAccumulatorLight) * 20.0f;
	FShadowRenderer::s_pointLight.m_position.m_y = 26.0f;
	FShadowRenderer::s_pointLight.m_position.m_z = bx::sin(FShadowRenderer::s_timeAccumulatorLight) * 20.0f;
	FShadowRenderer::s_pointLight.m_spotDirectionInner.m_x = -FShadowRenderer::s_pointLight.m_position.m_x;
	FShadowRenderer::s_pointLight.m_spotDirectionInner.m_y = -FShadowRenderer::s_pointLight.m_position.m_y;
	FShadowRenderer::s_pointLight.m_spotDirectionInner.m_z = -FShadowRenderer::s_pointLight.m_position.m_z;

	FShadowRenderer::s_directionalLight.m_position.m_x = -bx::cos(FShadowRenderer::s_timeAccumulatorLight);
	FShadowRenderer::s_directionalLight.m_position.m_y = -1.0f;
	FShadowRenderer::s_directionalLight.m_position.m_z = -bx::sin(FShadowRenderer::s_timeAccumulatorLight);
}

}