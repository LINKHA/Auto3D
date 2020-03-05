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
ALightComponent* FShadowRenderer::s_pointLight;
ALightComponent* FShadowRenderer::s_directionalLight;
float FShadowRenderer::s_color[4];
float FShadowRenderer::s_lightMtx[16];
float FShadowRenderer::s_shadowMapMtx[ShadowMapRenderTargets::Count][16];

ClearValues FShadowRenderer::s_clearValues;

ACameraComponent* FShadowRenderer::s_camera;

float FShadowRenderer::s_timeAccumulatorLight;
float FShadowRenderer::s_timeAccumulatorScene;

uint8_t FShadowRenderer::s_shadowMapPasses;
float FShadowRenderer::s_lightView[4][16];
float FShadowRenderer::s_lightProj[4][16];
float FShadowRenderer::s_mtxYpr[4][16];

float FShadowRenderer::s_screenProj[16];
float FShadowRenderer::s_screenView[16];
bgfx::VertexLayout FShadowRenderer::s_posLayout;



bgfx::VertexLayout PosColorTexCoord0Vertex::ms_layout;


void screenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft, float _width, float _height)
{
	if (3 == bgfx::getAvailTransientVertexBuffer(3, PosColorTexCoord0Vertex::ms_layout))
	{
		bgfx::TransientVertexBuffer vb;
		bgfx::allocTransientVertexBuffer(&vb, 3, PosColorTexCoord0Vertex::ms_layout);
		PosColorTexCoord0Vertex* vertex = (PosColorTexCoord0Vertex*)vb.data;

		const float zz = 0.0f;

		const float minx = -_width;
		const float maxx = _width;
		const float miny = 0.0f;
		const float maxy = _height * 2.0f;

		const float texelHalfW = FShadowRenderer::s_texelHalf / _textureWidth;
		const float texelHalfH = FShadowRenderer::s_texelHalf / _textureHeight;
		const float minu = -1.0f + texelHalfW;
		const float maxu = 1.0f + texelHalfW;

		float minv = texelHalfH;
		float maxv = 2.0f + texelHalfH;

		if (_originBottomLeft)
		{
			std::swap(minv, maxv);
			minv -= 1.0f;
			maxv -= 1.0f;
		}

		vertex[0].m_x = minx;
		vertex[0].m_y = miny;
		vertex[0].m_z = zz;
		vertex[0].m_rgba = 0xffffffff;
		vertex[0].m_u = minu;
		vertex[0].m_v = minv;

		vertex[1].m_x = maxx;
		vertex[1].m_y = miny;
		vertex[1].m_z = zz;
		vertex[1].m_rgba = 0xffffffff;
		vertex[1].m_u = maxu;
		vertex[1].m_v = minv;

		vertex[2].m_x = maxx;
		vertex[2].m_y = maxy;
		vertex[2].m_z = zz;
		vertex[2].m_rgba = 0xffffffff;
		vertex[2].m_u = maxu;
		vertex[2].m_v = maxv;

		bgfx::setVertexBuffer(0, &vb);
	}
}

void mtxYawPitchRoll(float* __restrict _result
	, float _yaw
	, float _pitch
	, float _roll
)
{
	float sroll = bx::sin(_roll);
	float croll = bx::cos(_roll);
	float spitch = bx::sin(_pitch);
	float cpitch = bx::cos(_pitch);
	float syaw = bx::sin(_yaw);
	float cyaw = bx::cos(_yaw);

	_result[0] = sroll * spitch * syaw + croll * cyaw;
	_result[1] = sroll * cpitch;
	_result[2] = sroll * spitch * cyaw - croll * syaw;
	_result[3] = 0.0f;
	_result[4] = croll * spitch * syaw - sroll * cyaw;
	_result[5] = croll * cpitch;
	_result[6] = croll * spitch * cyaw + sroll * syaw;
	_result[7] = 0.0f;
	_result[8] = cpitch * syaw;
	_result[9] = -spitch;
	_result[10] = cpitch * cyaw;
	_result[11] = 0.0f;
	_result[12] = 0.0f;
	_result[13] = 0.0f;
	_result[14] = 0.0f;
	_result[15] = 1.0f;
}

void worldSpaceFrustumCorners(float* _corners24f
	, float _near
	, float _far
	, float _projWidth
	, float _projHeight
	, const float* __restrict _invViewMtx
)
{
	// Define frustum corners in view space.
	const float nw = _near * _projWidth;
	const float nh = _near * _projHeight;
	const float fw = _far * _projWidth;
	const float fh = _far * _projHeight;

	const uint8_t numCorners = 8;
	const bx::Vec3 corners[numCorners] =
	{
		{ -nw,  nh, _near },
		{  nw,  nh, _near },
		{  nw, -nh, _near },
		{ -nw, -nh, _near },
		{ -fw,  fh, _far  },
		{  fw,  fh, _far  },
		{  fw, -fh, _far  },
		{ -fw, -fh, _far  },
	};

	// Convert them to world space.
	float(*out)[3] = (float(*)[3])_corners24f;
	for (uint8_t ii = 0; ii < numCorners; ++ii)
	{
		bx::store(&out[ii], bx::mul(corners[ii], _invViewMtx));
	}
}

/**
 * _splits = { near0, far0, near1, far1... nearN, farN }
 * N = _numSplits
 */
void splitFrustum(float* _splits, uint8_t _numSplits, float _near, float _far, float _splitWeight)
{
	const float l = _splitWeight;
	const float ratio = _far / _near;
	const int8_t numSlices = _numSplits * 2;
	const float numSlicesf = float(numSlices);

	// First slice.
	_splits[0] = _near;

	for (uint8_t nn = 2, ff = 1; nn < numSlices; nn += 2, ff += 2)
	{
		float si = float(int8_t(ff)) / numSlicesf;

		const float nearp = l * (_near*bx::pow(ratio, si)) + (1 - l)*(_near + (_far - _near)*si);
		_splits[nn] = nearp;          //near
		_splits[ff] = nearp * 1.005f; //far from previous split
	}

	// Last slice.
	_splits[numSlices - 1] = _far;
}


FShadowRenderer::FShadowRenderer() 
{}
FShadowRenderer::~FShadowRenderer() 
{}
void FShadowRenderer::init()
{
	GProcessWindow& processWindow = GProcessWindow::Get();

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
	s_pointLight->m_position = { 0.0f, 0.0f, 0.0f, 1.0f };
	s_pointLight->m_position_viewSpace[0] = 0.0f;
	s_pointLight->m_position_viewSpace[1] = 0.0f;
	s_pointLight->m_position_viewSpace[2] = 0.0f;
	s_pointLight->m_position_viewSpace[3] = 0.0f;
	s_pointLight->m_ambientPower = { 1.0f, 1.0f, 1.0f, 0.0f };
	s_pointLight->m_diffusePower = { 1.0f, 1.0f, 1.0f, 850.0f };
	s_pointLight->m_specularPower = { 1.0f, 1.0f, 1.0f, 0.0f };
	s_pointLight->m_spotDirectionInner = { 0.0f,-0.4f,-0.6f, 0.0f };
	s_pointLight->m_spotDirectionInner_viewSpace[0] = 0.0f;
	s_pointLight->m_spotDirectionInner_viewSpace[1] = 0.0f;
	s_pointLight->m_spotDirectionInner_viewSpace[2] = 0.0f;
	s_pointLight->m_spotDirectionInner_viewSpace[3] = 0.0f;
	s_pointLight->m_attenuationSpotOuter = { 1.0f, 0.0f, 1.0f, 91.0f };
	//s_pointLight =
	//{
	//	{ { 0.0f, 0.0f, 0.0f, 1.0f   } }, //position
	//	{   0.0f, 0.0f, 0.0f, 0.0f     }, //-ignore
	//	{ { 1.0f, 1.0f, 1.0f, 0.0f   } }, //ambient
	//	{ { 1.0f, 1.0f, 1.0f, 850.0f } }, //diffuse
	//	{ { 1.0f, 1.0f, 1.0f, 0.0f   } }, //specular
	//	{ { 0.0f,-0.4f,-0.6f, 0.0f   } }, //spotdirection, spotexponent
	//	{   0.0f, 0.0f, 0.0f, 0.0f     }, //-ignore
	//	{ { 1.0f, 0.0f, 1.0f, 91.0f  } }, //attenuation, spotcutoff
	//};

	s_directionalLight->m_position = { 0.5f,-1.0f, 0.1f, 0.0f };
	s_directionalLight->m_position_viewSpace[0] = 0.0f;
	s_directionalLight->m_position_viewSpace[1] = 0.0f;
	s_directionalLight->m_position_viewSpace[2] = 0.0f;
	s_directionalLight->m_position_viewSpace[3] = 0.0f;
	s_directionalLight->m_ambientPower = { 1.0f, 1.0f, 1.0f, 0.02f };
	s_directionalLight->m_diffusePower = { 1.0f, 1.0f, 1.0f, 0.4f };
	s_directionalLight->m_specularPower = { 1.0f, 1.0f, 1.0f, 0.0f };
	s_directionalLight->m_spotDirectionInner = { 0.0f, 0.0f, 0.0f, 1.0f };
	s_directionalLight->m_spotDirectionInner_viewSpace[0] = 0.0f;
	s_directionalLight->m_spotDirectionInner_viewSpace[1] = 0.0f;
	s_directionalLight->m_spotDirectionInner_viewSpace[2] = 0.0f;
	s_directionalLight->m_spotDirectionInner_viewSpace[3] = 0.0f;
	s_directionalLight->m_attenuationSpotOuter = { 0.0f, 0.0f, 0.0f, 1.0f };

	//s_directionalLight =
	//{
	//	{ { 0.5f,-1.0f, 0.1f, 0.0f  } }, //position
	//	{   0.0f, 0.0f, 0.0f, 0.0f    }, //-ignore
	//	{ { 1.0f, 1.0f, 1.0f, 0.02f } }, //ambient
	//	{ { 1.0f, 1.0f, 1.0f, 0.4f  } }, //diffuse
	//	{ { 1.0f, 1.0f, 1.0f, 0.0f  } }, //specular
	//	{ { 0.0f, 0.0f, 0.0f, 1.0f  } }, //spotdirection, spotexponent
	//	{   0.0f, 0.0f, 0.0f, 0.0f    }, //-ignore
	//	{ { 0.0f, 0.0f, 0.0f, 1.0f  } }, //attenuation, spotcutoff
	//};

	// Setup uniforms.
	FShadowRenderer::s_color[0] = FShadowRenderer::s_color[1] = FShadowRenderer::s_color[2] = FShadowRenderer::s_color[3] = 1.0f;
	FShadowRenderer::s_uniforms.setPtrs(&FShadowRenderer::s_defaultMaterial
		, FShadowRenderer::s_pointLight
		, FShadowRenderer::s_color
		, FShadowRenderer::s_lightMtx
		, &FShadowRenderer::s_shadowMapMtx[ShadowMapRenderTargets::First][0]
		, &FShadowRenderer::s_shadowMapMtx[ShadowMapRenderTargets::Second][0]
		, &FShadowRenderer::s_shadowMapMtx[ShadowMapRenderTargets::Third][0]
		, &FShadowRenderer::s_shadowMapMtx[ShadowMapRenderTargets::Fourth][0]
	);

	FShadowRenderer::s_timeAccumulatorLight = 0.0f;
	FShadowRenderer::s_timeAccumulatorScene = 0.0f;


	FShadowRenderer::s_posLayout.begin();
	FShadowRenderer::s_posLayout.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
	FShadowRenderer::s_posLayout.end();

	PosColorTexCoord0Vertex::init();
}
void FShadowRenderer::update()
{
	GProcessWindow& processWindow = GProcessWindow::Get();

	processWindow._width = uint16_t(processWindow._width);
	processWindow._height = uint16_t(processWindow._height);

	const bgfx::Caps* caps = bgfx::getCaps();

	// Set view and projection matrices.
	const float camFovy = s_camera->GetFov();
	const float camAspect = s_camera->GetAspectRatio();
	const float projHeight = bx::tan(bx::toRad(camFovy)*0.5f);
	const float projWidth = projHeight * camAspect;

	s_camera->SetAspectRatio(float(GProcessWindow::Get()._width) / float(GProcessWindow::Get()._height));

	TMatrix4x4F projectionMatrix = s_camera->GetProjectionMatrix();
	TMatrix3x4F viewMatrix = s_camera->GetViewMatrix();
	TMatrix4x4F transposeViewMatrix = viewMatrix.ToMatrix4().Transpose();


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
	FShadowRenderer::s_uniforms.m_lightPtr = (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType) ? FShadowRenderer::s_directionalLight : FShadowRenderer::s_pointLight;

	if (LightType::SpotLight == FShadowRenderer::s_settings.m_lightType)
	{
		FShadowRenderer::s_pointLight->m_attenuationSpotOuter.m_outer = FShadowRenderer::s_settings.m_spotOuterAngle;
		FShadowRenderer::s_pointLight->m_spotDirectionInner.m_inner = FShadowRenderer::s_settings.m_spotInnerAngle;
	}
	else
	{
		FShadowRenderer::s_pointLight->m_attenuationSpotOuter.m_outer = 91.0f; //above 90.0f means point light
	}

	FShadowRenderer::s_uniforms.submitPerFrameUniforms();

	int64_t now = bx::getHPCounter();
	static int64_t last = now;
	const int64_t frameTime = now - last;
	last = now;
	const double freq = double(bx::getHPFrequency());
	const float deltaTime = float(frameTime / freq);

	// Update lights.
	FShadowRenderer::s_pointLight->computeViewSpaceComponents(transposeViewMatrix.Data());
	FShadowRenderer::s_directionalLight->computeViewSpaceComponents(transposeViewMatrix.Data());

	// Update time accumulators.
	if (FShadowRenderer::s_settings.m_updateLights) { FShadowRenderer::s_timeAccumulatorLight += deltaTime; }
	if (FShadowRenderer::s_settings.m_updateScene) { FShadowRenderer::s_timeAccumulatorScene += deltaTime; }

	// Setup lights.
	FShadowRenderer::s_pointLight->m_position.m_x = bx::cos(FShadowRenderer::s_timeAccumulatorLight) * 20.0f;
	FShadowRenderer::s_pointLight->m_position.m_y = 26.0f;
	FShadowRenderer::s_pointLight->m_position.m_z = bx::sin(FShadowRenderer::s_timeAccumulatorLight) * 20.0f;
	FShadowRenderer::s_pointLight->m_spotDirectionInner.m_x = -FShadowRenderer::s_pointLight->m_position.m_x;
	FShadowRenderer::s_pointLight->m_spotDirectionInner.m_y = -FShadowRenderer::s_pointLight->m_position.m_y;
	FShadowRenderer::s_pointLight->m_spotDirectionInner.m_z = -FShadowRenderer::s_pointLight->m_position.m_z;

	FShadowRenderer::s_directionalLight->m_position.m_x = -bx::cos(FShadowRenderer::s_timeAccumulatorLight);
	FShadowRenderer::s_directionalLight->m_position.m_y = -1.0f;
	FShadowRenderer::s_directionalLight->m_position.m_z = -bx::sin(FShadowRenderer::s_timeAccumulatorLight);

	bx::mtxIdentity(FShadowRenderer::s_screenView);
	bx::mtxOrtho(
		FShadowRenderer::s_screenProj
		, 0.0f
		, 1.0f
		, 1.0f
		, 0.0f
		, 0.0f
		, 100.0f
		, 0.0f
		, caps->homogeneousDepth
	);

	// Update render target size.
	uint16_t shadowMapSize = 1 << uint32_t(currentShadowMapSettings->m_sizePwrTwo);
	if (FShadowRenderer::s_currentShadowMapSize != shadowMapSize)
	{
		FShadowRenderer::s_currentShadowMapSize = shadowMapSize;
		FShadowRenderer::s_uniforms.m_shadowMapTexelSize = 1.0f / currentShadowMapSizef;

		{
			bgfx::destroy(FShadowRenderer::s_rtShadowMap[0]);

			bgfx::TextureHandle fbtextures[] =
			{
				bgfx::createTexture2D(FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT),
				bgfx::createTexture2D(FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize, false, 1, bgfx::TextureFormat::D24S8, BGFX_TEXTURE_RT),
			};
			FShadowRenderer::s_rtShadowMap[0] = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
		}

		if (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType)
		{
			for (uint8_t ii = 1; ii < ShadowMapRenderTargets::Count; ++ii)
			{
				{
					bgfx::destroy(FShadowRenderer::s_rtShadowMap[ii]);

					bgfx::TextureHandle fbtextures[] =
					{
						bgfx::createTexture2D(FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT),
						bgfx::createTexture2D(FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize, false, 1, bgfx::TextureFormat::D24S8, BGFX_TEXTURE_RT),
					};
					FShadowRenderer::s_rtShadowMap[ii] = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
				}
			}
		}

		bgfx::destroy(FShadowRenderer::s_rtBlur);
		FShadowRenderer::s_rtBlur = bgfx::createFrameBuffer(FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize, bgfx::TextureFormat::BGRA8);
	}

	if (LightType::SpotLight == FShadowRenderer::s_settings.m_lightType)
	{
		const float fovy = FShadowRenderer::s_settings.m_coverageSpotL;
		const float aspect = 1.0f;
		bx::mtxProj(
			FShadowRenderer::s_lightProj[ProjType::Horizontal]
			, fovy
			, aspect
			, currentShadowMapSettings->m_near
			, currentShadowMapSettings->m_far
			, false
		);

		//For linear depth, prevent depth division by variable w-component in shaders and divide here by far plane
		if (DepthImpl::Linear == FShadowRenderer::s_settings.m_depthImpl)
		{
			FShadowRenderer::s_lightProj[ProjType::Horizontal][10] /= currentShadowMapSettings->m_far;
			FShadowRenderer::s_lightProj[ProjType::Horizontal][14] /= currentShadowMapSettings->m_far;
		}

		const bx::Vec3 at = bx::add(bx::load<bx::Vec3>(FShadowRenderer::s_pointLight->m_position.m_v), bx::load<bx::Vec3>(FShadowRenderer::s_pointLight->m_spotDirectionInner.m_v));
		bx::mtxLookAt(FShadowRenderer::s_lightView[TetrahedronFaces::Green], bx::load<bx::Vec3>(FShadowRenderer::s_pointLight->m_position.m_v), at);
	}
	else if (LightType::PointLight == FShadowRenderer::s_settings.m_lightType)
	{
		float ypr[TetrahedronFaces::Count][3] =
		{
			{ bx::toRad(0.0f), bx::toRad(27.36780516f), bx::toRad(0.0f) },
			{ bx::toRad(180.0f), bx::toRad(27.36780516f), bx::toRad(0.0f) },
			{ bx::toRad(-90.0f), bx::toRad(-27.36780516f), bx::toRad(0.0f) },
			{ bx::toRad(90.0f), bx::toRad(-27.36780516f), bx::toRad(0.0f) },
		};


		if (FShadowRenderer::s_settings.m_stencilPack)
		{
			const float fovx = 143.98570868f + 3.51f + FShadowRenderer::s_settings.m_fovXAdjust;
			const float fovy = 125.26438968f + 9.85f + FShadowRenderer::s_settings.m_fovYAdjust;
			const float aspect = bx::tan(bx::toRad(fovx*0.5f)) / bx::tan(bx::toRad(fovy*0.5f));

			bx::mtxProj(
				FShadowRenderer::s_lightProj[ProjType::Vertical]
				, fovx
				, aspect
				, currentShadowMapSettings->m_near
				, currentShadowMapSettings->m_far
				, false
			);

			//For linear depth, prevent depth division by variable w-component in shaders and divide here by far plane
			if (DepthImpl::Linear == FShadowRenderer::s_settings.m_depthImpl)
			{
				FShadowRenderer::s_lightProj[ProjType::Vertical][10] /= currentShadowMapSettings->m_far;
				FShadowRenderer::s_lightProj[ProjType::Vertical][14] /= currentShadowMapSettings->m_far;
			}

			ypr[TetrahedronFaces::Green][2] = bx::toRad(180.0f);
			ypr[TetrahedronFaces::Yellow][2] = bx::toRad(0.0f);
			ypr[TetrahedronFaces::Blue][2] = bx::toRad(90.0f);
			ypr[TetrahedronFaces::Red][2] = bx::toRad(-90.0f);
		}

		const float fovx = 143.98570868f + 7.8f + FShadowRenderer::s_settings.m_fovXAdjust;
		const float fovy = 125.26438968f + 3.0f + FShadowRenderer::s_settings.m_fovYAdjust;
		const float aspect = bx::tan(bx::toRad(fovx*0.5f)) / bx::tan(bx::toRad(fovy*0.5f));

		bx::mtxProj(
			FShadowRenderer::s_lightProj[ProjType::Horizontal]
			, fovy
			, aspect
			, currentShadowMapSettings->m_near
			, currentShadowMapSettings->m_far
			, caps->homogeneousDepth
		);

		//For linear depth, prevent depth division by variable w component in shaders and divide here by far plane
		if (DepthImpl::Linear == FShadowRenderer::s_settings.m_depthImpl)
		{
			FShadowRenderer::s_lightProj[ProjType::Horizontal][10] /= currentShadowMapSettings->m_far;
			FShadowRenderer::s_lightProj[ProjType::Horizontal][14] /= currentShadowMapSettings->m_far;
		}


		for (uint8_t ii = 0; ii < TetrahedronFaces::Count; ++ii)
		{
			float mtxTmp[16];
			mtxYawPitchRoll(mtxTmp, ypr[ii][0], ypr[ii][1], ypr[ii][2]);

			float tmp[3] =
			{
				-bx::dot(bx::load<bx::Vec3>(FShadowRenderer::s_pointLight->m_position.m_v), bx::load<bx::Vec3>(&mtxTmp[0])),
				-bx::dot(bx::load<bx::Vec3>(FShadowRenderer::s_pointLight->m_position.m_v), bx::load<bx::Vec3>(&mtxTmp[4])),
				-bx::dot(bx::load<bx::Vec3>(FShadowRenderer::s_pointLight->m_position.m_v), bx::load<bx::Vec3>(&mtxTmp[8])),
			};

			bx::mtxTranspose(FShadowRenderer::s_mtxYpr[ii], mtxTmp);

			bx::memCopy(FShadowRenderer::s_lightView[ii], FShadowRenderer::s_mtxYpr[ii], 12 * sizeof(float));
			FShadowRenderer::s_lightView[ii][12] = tmp[0];
			FShadowRenderer::s_lightView[ii][13] = tmp[1];
			FShadowRenderer::s_lightView[ii][14] = tmp[2];
			FShadowRenderer::s_lightView[ii][15] = 1.0f;
		}
	}
	else // LightType::DirectionalLight == settings.m_lightType
	{
		// Setup light view mtx.
		const bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
		const bx::Vec3 eye =
		{
			-FShadowRenderer::s_directionalLight->m_position.m_x,
			-FShadowRenderer::s_directionalLight->m_position.m_y,
			-FShadowRenderer::s_directionalLight->m_position.m_z,
		};
		bx::mtxLookAt(FShadowRenderer::s_lightView[0], eye, at);

		// Compute camera inverse view mtx.
		float mtxViewInv[16];
		bx::mtxInverse(mtxViewInv, transposeViewMatrix.Data());

		// Compute split distances.
		const uint8_t maxNumSplits = 4;
		BX_CHECK(maxNumSplits >= settings.m_numSplits, "Error! Max num splits.");

		float splitSlices[maxNumSplits * 2];
		splitFrustum(splitSlices
			, uint8_t(FShadowRenderer::s_settings.m_numSplits)
			, currentShadowMapSettings->m_near
			, currentShadowMapSettings->m_far
			, FShadowRenderer::s_settings.m_splitDistribution
		);

		// Update uniforms.
		for (uint8_t ii = 0, ff = 1; ii < FShadowRenderer::s_settings.m_numSplits; ++ii, ff += 2)
		{
			// This lags for 1 frame, but it's not a problem.
			FShadowRenderer::s_uniforms.m_csmFarDistances[ii] = splitSlices[ff];
		}

		float mtxProj[16];
		bx::mtxOrtho(
			mtxProj
			, 1.0f
			, -1.0f
			, 1.0f
			, -1.0f
			, -currentShadowMapSettings->m_far
			, currentShadowMapSettings->m_far
			, 0.0f
			, caps->homogeneousDepth
		);

		const uint8_t numCorners = 8;
		float frustumCorners[maxNumSplits][numCorners][3];
		for (uint8_t ii = 0, nn = 0, ff = 1; ii < FShadowRenderer::s_settings.m_numSplits; ++ii, nn += 2, ff += 2)
		{
			// Compute frustum corners for one split in world space.
			worldSpaceFrustumCorners((float*)frustumCorners[ii], splitSlices[nn], splitSlices[ff], projWidth, projHeight, mtxViewInv);

			bx::Vec3 min = { 9000.0f,  9000.0f,  9000.0f };
			bx::Vec3 max = { -9000.0f, -9000.0f, -9000.0f };

			for (uint8_t jj = 0; jj < numCorners; ++jj)
			{
				// Transform to light space.
				const bx::Vec3 xyz = bx::mul(bx::load<bx::Vec3>(frustumCorners[ii][jj]), FShadowRenderer::s_lightView[0]);

				// Update bounding box.
				min = bx::min(min, xyz);
				max = bx::max(max, xyz);
			}

			const bx::Vec3 minproj = bx::mulH(min, mtxProj);
			const bx::Vec3 maxproj = bx::mulH(max, mtxProj);

			float scalex = 2.0f / (maxproj.x - minproj.x);
			float scaley = 2.0f / (maxproj.y - minproj.y);

			if (FShadowRenderer::s_settings.m_stabilize)
			{
				const float quantizer = 64.0f;
				scalex = quantizer / bx::ceil(quantizer / scalex);
				scaley = quantizer / bx::ceil(quantizer / scaley);
			}

			float offsetx = 0.5f * (maxproj.x + minproj.x) * scalex;
			float offsety = 0.5f * (maxproj.y + minproj.y) * scaley;

			if (FShadowRenderer::s_settings.m_stabilize)
			{
				const float halfSize = currentShadowMapSizef * 0.5f;
				offsetx = bx::ceil(offsetx * halfSize) / halfSize;
				offsety = bx::ceil(offsety * halfSize) / halfSize;
			}

			float mtxCrop[16];
			bx::mtxIdentity(mtxCrop);
			mtxCrop[0] = scalex;
			mtxCrop[5] = scaley;
			mtxCrop[12] = offsetx;
			mtxCrop[13] = offsety;

			bx::mtxMul(FShadowRenderer::s_lightProj[ii], mtxCrop, mtxProj);
		}
	}


	// Reset render targets.
	const bgfx::FrameBufferHandle invalidRt = BGFX_INVALID_HANDLE;
	for (uint8_t ii = 0; ii < RENDERVIEW_DRAWDEPTH_3_ID + 1; ++ii)
	{
		bgfx::setViewFrameBuffer(ii, invalidRt);
		bgfx::setViewRect(ii, 0, 0, processWindow._width, processWindow._height);
	}

	// Determine on-screen rectangle size where depth buffer will be drawn.
	uint16_t depthRectHeight = uint16_t(float(processWindow._height) / 2.5f);
	uint16_t depthRectWidth = depthRectHeight;
	uint16_t depthRectX = 0;
	uint16_t depthRectY = processWindow._height - depthRectHeight;

	// Setup views and render targets.
	bgfx::setViewRect(0, 0, 0, processWindow._width, processWindow._height);
	bgfx::setViewTransform(0, transposeViewMatrix.Data(), projectionMatrix.Data());

	if (LightType::SpotLight == FShadowRenderer::s_settings.m_lightType)
	{
		/**
			* RENDERVIEW_SHADOWMAP_0_ID - Clear shadow map. (used as convenience, otherwise render_pass_1 could be cleared)
			* RENDERVIEW_SHADOWMAP_1_ID - Craft shadow map.
			* RENDERVIEW_VBLUR_0_ID - Vertical blur.
			* RENDERVIEW_HBLUR_0_ID - Horizontal blur.
			* RENDERVIEW_DRAWSCENE_0_ID - Draw scene.
			* RENDERVIEW_DRAWSCENE_1_ID - Draw floor bottom.
			* RENDERVIEW_DRAWDEPTH_0_ID - Draw depth buffer.
			*/

		bgfx::setViewRect(RENDERVIEW_SHADOWMAP_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_SHADOWMAP_1_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_VBLUR_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_HBLUR_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_DRAWSCENE_0_ID, 0, 0, processWindow._width, processWindow._height);
		bgfx::setViewRect(RENDERVIEW_DRAWSCENE_1_ID, 0, 0, processWindow._width, processWindow._height);
		bgfx::setViewRect(RENDERVIEW_DRAWDEPTH_0_ID, depthRectX, depthRectY, depthRectWidth, depthRectHeight);

		bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_0_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_1_ID, FShadowRenderer::s_lightView[0], FShadowRenderer::s_lightProj[ProjType::Horizontal]);
		bgfx::setViewTransform(RENDERVIEW_VBLUR_0_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_HBLUR_0_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_DRAWSCENE_0_ID, transposeViewMatrix.Data(), projectionMatrix.Data());
		bgfx::setViewTransform(RENDERVIEW_DRAWSCENE_1_ID, transposeViewMatrix.Data(), projectionMatrix.Data());
		bgfx::setViewTransform(RENDERVIEW_DRAWDEPTH_0_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);

		bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_0_ID, FShadowRenderer::s_rtShadowMap[0]);
		bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_1_ID, FShadowRenderer::s_rtShadowMap[0]);
		bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_0_ID, FShadowRenderer::s_rtBlur);
		bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_0_ID, FShadowRenderer::s_rtShadowMap[0]);
	}
	else if (LightType::PointLight == FShadowRenderer::s_settings.m_lightType)
	{
		/**
			* RENDERVIEW_SHADOWMAP_0_ID - Clear entire shadow map.
			* RENDERVIEW_SHADOWMAP_1_ID - Craft green tetrahedron shadow face.
			* RENDERVIEW_SHADOWMAP_2_ID - Craft yellow tetrahedron shadow face.
			* RENDERVIEW_SHADOWMAP_3_ID - Craft blue tetrahedron shadow face.
			* RENDERVIEW_SHADOWMAP_4_ID - Craft red tetrahedron shadow face.
			* RENDERVIEW_VBLUR_0_ID - Vertical blur.
			* RENDERVIEW_HBLUR_0_ID - Horizontal blur.
			* RENDERVIEW_DRAWSCENE_0_ID - Draw scene.
			* RENDERVIEW_DRAWSCENE_1_ID - Draw floor bottom.
			* RENDERVIEW_DRAWDEPTH_0_ID - Draw depth buffer.
			*/

		bgfx::setViewRect(RENDERVIEW_SHADOWMAP_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		if (FShadowRenderer::s_settings.m_stencilPack)
		{
			const uint16_t f = FShadowRenderer::s_currentShadowMapSize;   //full size
			const uint16_t h = FShadowRenderer::s_currentShadowMapSize / 2; //half size
			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_1_ID, 0, 0, f, h);
			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_2_ID, 0, h, f, h);
			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_3_ID, 0, 0, h, f);
			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_4_ID, h, 0, h, f);
		}
		else
		{
			const uint16_t h = FShadowRenderer::s_currentShadowMapSize / 2; //half size
			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_1_ID, 0, 0, h, h);
			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_2_ID, h, 0, h, h);
			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_3_ID, 0, h, h, h);
			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_4_ID, h, h, h, h);
		}
		bgfx::setViewRect(RENDERVIEW_VBLUR_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_HBLUR_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_DRAWSCENE_0_ID, 0, 0, processWindow._width, processWindow._height);
		bgfx::setViewRect(RENDERVIEW_DRAWSCENE_1_ID, 0, 0, processWindow._width, processWindow._height);
		bgfx::setViewRect(RENDERVIEW_DRAWDEPTH_0_ID, depthRectX, depthRectY, depthRectWidth, depthRectHeight);

		bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_0_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_1_ID, FShadowRenderer::s_lightView[TetrahedronFaces::Green], FShadowRenderer::s_lightProj[ProjType::Horizontal]);
		bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_2_ID, FShadowRenderer::s_lightView[TetrahedronFaces::Yellow], FShadowRenderer::s_lightProj[ProjType::Horizontal]);
		if (FShadowRenderer::s_settings.m_stencilPack)
		{
			bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_3_ID, FShadowRenderer::s_lightView[TetrahedronFaces::Blue], FShadowRenderer::s_lightProj[ProjType::Vertical]);
			bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_4_ID, FShadowRenderer::s_lightView[TetrahedronFaces::Red], FShadowRenderer::s_lightProj[ProjType::Vertical]);
		}
		else
		{
			bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_3_ID, FShadowRenderer::s_lightView[TetrahedronFaces::Blue], FShadowRenderer::s_lightProj[ProjType::Horizontal]);
			bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_4_ID, FShadowRenderer::s_lightView[TetrahedronFaces::Red], FShadowRenderer::s_lightProj[ProjType::Horizontal]);
		}
		bgfx::setViewTransform(RENDERVIEW_VBLUR_0_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_HBLUR_0_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_DRAWSCENE_0_ID, transposeViewMatrix.Data(), projectionMatrix.Data());
		bgfx::setViewTransform(RENDERVIEW_DRAWSCENE_1_ID, transposeViewMatrix.Data(), projectionMatrix.Data());
		bgfx::setViewTransform(RENDERVIEW_DRAWDEPTH_0_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);

		bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_0_ID, FShadowRenderer::s_rtShadowMap[0]);
		bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_1_ID, FShadowRenderer::s_rtShadowMap[0]);
		bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_2_ID, FShadowRenderer::s_rtShadowMap[0]);
		bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_3_ID, FShadowRenderer::s_rtShadowMap[0]);
		bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_4_ID, FShadowRenderer::s_rtShadowMap[0]);
		bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_0_ID, FShadowRenderer::s_rtBlur);
		bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_0_ID, FShadowRenderer::s_rtShadowMap[0]);
	}
	else // LightType::DirectionalLight == settings.m_lightType
	{
		/**
			* RENDERVIEW_SHADOWMAP_1_ID - Craft shadow map for first  split.
			* RENDERVIEW_SHADOWMAP_2_ID - Craft shadow map for second split.
			* RENDERVIEW_SHADOWMAP_3_ID - Craft shadow map for third  split.
			* RENDERVIEW_SHADOWMAP_4_ID - Craft shadow map for fourth split.
			* RENDERVIEW_VBLUR_0_ID - Vertical   blur for first  split.
			* RENDERVIEW_HBLUR_0_ID - Horizontal blur for first  split.
			* RENDERVIEW_VBLUR_1_ID - Vertical   blur for second split.
			* RENDERVIEW_HBLUR_1_ID - Horizontal blur for second split.
			* RENDERVIEW_VBLUR_2_ID - Vertical   blur for third  split.
			* RENDERVIEW_HBLUR_2_ID - Horizontal blur for third  split.
			* RENDERVIEW_VBLUR_3_ID - Vertical   blur for fourth split.
			* RENDERVIEW_HBLUR_3_ID - Horizontal blur for fourth split.
			* RENDERVIEW_DRAWSCENE_0_ID - Draw scene.
			* RENDERVIEW_DRAWSCENE_1_ID - Draw floor bottom.
			* RENDERVIEW_DRAWDEPTH_0_ID - Draw depth buffer for first  split.
			* RENDERVIEW_DRAWDEPTH_1_ID - Draw depth buffer for second split.
			* RENDERVIEW_DRAWDEPTH_2_ID - Draw depth buffer for third  split.
			* RENDERVIEW_DRAWDEPTH_3_ID - Draw depth buffer for fourth split.
			*/

		depthRectHeight = processWindow._height / 3;
		depthRectWidth = depthRectHeight;
		depthRectX = 0;
		depthRectY = processWindow._height - depthRectHeight;

		bgfx::setViewRect(RENDERVIEW_SHADOWMAP_1_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_SHADOWMAP_2_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_SHADOWMAP_3_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_SHADOWMAP_4_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_VBLUR_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_HBLUR_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_VBLUR_1_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_HBLUR_1_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_VBLUR_2_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_HBLUR_2_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_VBLUR_3_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_HBLUR_3_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
		bgfx::setViewRect(RENDERVIEW_DRAWSCENE_0_ID, 0, 0, processWindow._width, processWindow._height);
		bgfx::setViewRect(RENDERVIEW_DRAWSCENE_1_ID, 0, 0, processWindow._width, processWindow._height);
		bgfx::setViewRect(RENDERVIEW_DRAWDEPTH_0_ID, depthRectX + (0 * depthRectWidth), depthRectY, depthRectWidth, depthRectHeight);
		bgfx::setViewRect(RENDERVIEW_DRAWDEPTH_1_ID, depthRectX + (1 * depthRectWidth), depthRectY, depthRectWidth, depthRectHeight);
		bgfx::setViewRect(RENDERVIEW_DRAWDEPTH_2_ID, depthRectX + (2 * depthRectWidth), depthRectY, depthRectWidth, depthRectHeight);
		bgfx::setViewRect(RENDERVIEW_DRAWDEPTH_3_ID, depthRectX + (3 * depthRectWidth), depthRectY, depthRectWidth, depthRectHeight);

		bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_1_ID, FShadowRenderer::s_lightView[0], FShadowRenderer::s_lightProj[0]);
		bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_2_ID, FShadowRenderer::s_lightView[0], FShadowRenderer::s_lightProj[1]);
		bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_3_ID, FShadowRenderer::s_lightView[0], FShadowRenderer::s_lightProj[2]);
		bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_4_ID, FShadowRenderer::s_lightView[0], FShadowRenderer::s_lightProj[3]);
		bgfx::setViewTransform(RENDERVIEW_VBLUR_0_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_HBLUR_0_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_VBLUR_1_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_HBLUR_1_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_VBLUR_2_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_HBLUR_2_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_VBLUR_3_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_HBLUR_3_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_DRAWSCENE_0_ID, transposeViewMatrix.Data(), projectionMatrix.Data());
		bgfx::setViewTransform(RENDERVIEW_DRAWSCENE_1_ID, transposeViewMatrix.Data(), projectionMatrix.Data());
		bgfx::setViewTransform(RENDERVIEW_DRAWDEPTH_0_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_DRAWDEPTH_1_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_DRAWDEPTH_2_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);
		bgfx::setViewTransform(RENDERVIEW_DRAWDEPTH_3_ID, FShadowRenderer::s_screenView, FShadowRenderer::s_screenProj);

		bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_1_ID, FShadowRenderer::s_rtShadowMap[0]);
		bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_2_ID, FShadowRenderer::s_rtShadowMap[1]);
		bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_3_ID, FShadowRenderer::s_rtShadowMap[2]);
		bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_4_ID, FShadowRenderer::s_rtShadowMap[3]);
		bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_0_ID, FShadowRenderer::s_rtBlur);         //vblur
		bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_0_ID, FShadowRenderer::s_rtShadowMap[0]); //hblur
		bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_1_ID, FShadowRenderer::s_rtBlur);         //vblur
		bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_1_ID, FShadowRenderer::s_rtShadowMap[1]); //hblur
		bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_2_ID, FShadowRenderer::s_rtBlur);         //vblur
		bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_2_ID, FShadowRenderer::s_rtShadowMap[2]); //hblur
		bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_3_ID, FShadowRenderer::s_rtBlur);         //vblur
		bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_3_ID, FShadowRenderer::s_rtShadowMap[3]); //hblur
	}


	// Clear backbuffer at beginning.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR
		| BGFX_CLEAR_DEPTH
		, FShadowRenderer::s_clearValues.m_clearRgba
		, FShadowRenderer::s_clearValues.m_clearDepth
		, FShadowRenderer::s_clearValues.m_clearStencil
	);
	bgfx::touch(0);

	// Clear shadowmap rendertarget at beginning.
	const uint8_t flags0 = (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType)
		? 0
		: BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL
		;

	bgfx::setViewClear(RENDERVIEW_SHADOWMAP_0_ID
		, flags0
		, 0xfefefefe //blur fails on completely white regions
		, FShadowRenderer::s_clearValues.m_clearDepth
		, FShadowRenderer::s_clearValues.m_clearStencil
	);
	bgfx::touch(RENDERVIEW_SHADOWMAP_0_ID);

	const uint8_t flags1 = (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType)
		? BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		: 0
		;

	for (uint8_t ii = 0; ii < 4; ++ii)
	{
		bgfx::setViewClear(RENDERVIEW_SHADOWMAP_1_ID + ii
			, flags1
			, 0xfefefefe //blur fails on completely white regions
			, FShadowRenderer::s_clearValues.m_clearDepth
			, FShadowRenderer::s_clearValues.m_clearStencil
		);
		bgfx::touch(RENDERVIEW_SHADOWMAP_1_ID + ii);
	}


	PackDepth::Enum depthType = (SmImpl::VSM == FShadowRenderer::s_settings.m_smImpl) ? PackDepth::VSM : PackDepth::RGBA;
	bool bVsmOrEsm = (SmImpl::VSM == FShadowRenderer::s_settings.m_smImpl) || (SmImpl::ESM == FShadowRenderer::s_settings.m_smImpl);

	// Blur shadow map.
	if (bVsmOrEsm
		&&  currentShadowMapSettings->m_doBlur)
	{
		bgfx::setTexture(4, FShadowRenderer::s_shadowMap[0], bgfx::getTexture(FShadowRenderer::s_rtShadowMap[0]));
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		screenSpaceQuad(currentShadowMapSizef, currentShadowMapSizef, FShadowRenderer::s_flipV);
		bgfx::submit(RENDERVIEW_VBLUR_0_ID, FShadowRenderer::s_programs.m_vBlur[depthType]);

		bgfx::setTexture(4, FShadowRenderer::s_shadowMap[0], bgfx::getTexture(FShadowRenderer::s_rtBlur));
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		screenSpaceQuad(currentShadowMapSizef, currentShadowMapSizef, FShadowRenderer::s_flipV);
		bgfx::submit(RENDERVIEW_HBLUR_0_ID, FShadowRenderer::s_programs.m_hBlur[depthType]);

		if (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType)
		{
			for (uint8_t ii = 1, jj = 2; ii < FShadowRenderer::s_settings.m_numSplits; ++ii, jj += 2)
			{
				const uint8_t viewId = RENDERVIEW_VBLUR_0_ID + jj;

				bgfx::setTexture(4, FShadowRenderer::s_shadowMap[0], bgfx::getTexture(FShadowRenderer::s_rtShadowMap[ii]));
				bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
				screenSpaceQuad(currentShadowMapSizef, currentShadowMapSizef, FShadowRenderer::s_flipV);
				bgfx::submit(viewId, FShadowRenderer::s_programs.m_vBlur[depthType]);

				bgfx::setTexture(4, FShadowRenderer::s_shadowMap[0], bgfx::getTexture(FShadowRenderer::s_rtBlur));
				bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
				screenSpaceQuad(currentShadowMapSizef, currentShadowMapSizef, FShadowRenderer::s_flipV);
				bgfx::submit(viewId + 1, FShadowRenderer::s_programs.m_hBlur[depthType]);
			}
		}
	}

	// Draw depth rect.
	if (FShadowRenderer::s_settings.m_drawDepthBuffer)
	{
		bgfx::setTexture(4, FShadowRenderer::s_shadowMap[0], bgfx::getTexture(FShadowRenderer::s_rtShadowMap[0]));
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		screenSpaceQuad(currentShadowMapSizef, currentShadowMapSizef, FShadowRenderer::s_flipV);
		bgfx::submit(RENDERVIEW_DRAWDEPTH_0_ID, FShadowRenderer::s_programs.m_drawDepth[depthType]);

		if (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType)
		{
			for (uint8_t ii = 1; ii < FShadowRenderer::s_settings.m_numSplits; ++ii)
			{
				bgfx::setTexture(4, FShadowRenderer::s_shadowMap[0], bgfx::getTexture(FShadowRenderer::s_rtShadowMap[ii]));
				bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
				screenSpaceQuad(currentShadowMapSizef, currentShadowMapSizef, FShadowRenderer::s_flipV);
				bgfx::submit(RENDERVIEW_DRAWDEPTH_0_ID + ii, FShadowRenderer::s_programs.m_drawDepth[depthType]);
			}
		}
	}

}

}