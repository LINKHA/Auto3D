#pragma once
#include "AutoConfig.h"
#include "RHI/bgfx_utils.h"
#include "Renderer/RendererDef.h"
#include "Resource/ResourceCache.h"
#include "Platform/ProcessWindow.h"
#include "Resource/Mesh.h"

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/rng.h>
#include <bx/timer.h>

namespace Auto3D
{
	/*void screenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft = false, float _width = 1.0f, float _height = 1.0f)
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

			const float texelHalfW = FRendererDef::s_texelHalf / _textureWidth;
			const float texelHalfH = FRendererDef::s_texelHalf / _textureHeight;
			const float minu = -1.0f + texelHalfW;
			const float maxu = 1.0f + texelHalfW;

			float minv = texelHalfH;
			float maxv = 2.0f + texelHalfH;

			if (_originBottomLeft)
			{
				float temp = minv;
				minv = maxv;
				maxv = temp;

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
	}*/

	void setOffsets2x2Lum(bgfx::UniformHandle _handle, uint32_t _width, uint32_t _height);

	void setOffsets4x4Lum(bgfx::UniformHandle _handle, uint32_t _width, uint32_t _height);

struct HDRSettings
{
	float m_speed;
	float m_middleGray;
	float m_white;
	float m_threshold;
};

class AUTO_API FHDRPipline
{
public:
	FHDRPipline();
	~FHDRPipline();
	void Init()
	{
		GResourceModule& reousceModlue = GResourceModule::Get();

		m_uffizi = loadTexture("textures/uffizi.ktx"
			, 0
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
			| BGFX_SAMPLER_W_CLAMP
		);

		m_skyProgram.AttachShader("vs_hdr_skybox", "fs_hdr_skybox");
		m_lumProgram.AttachShader("vs_hdr_lum", "fs_hdr_lum");
		m_lumAvgProgram.AttachShader("vs_hdr_lumavg", "fs_hdr_lumavg");
		m_blurProgram.AttachShader("vs_hdr_blur", "fs_hdr_blur");
		m_brightProgram.AttachShader("vs_hdr_bright", "fs_hdr_bright");
		m_meshProgram.AttachShader("vs_hdr_mesh", "fs_hdr_mesh");
		m_tonemapProgram.AttachShader("vs_hdr_tonemap", "fs_hdr_tonemap");

		s_texCube = bgfx::createUniform("s_texCube", bgfx::UniformType::Sampler);
		s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
		s_texLum = bgfx::createUniform("s_texLum", bgfx::UniformType::Sampler);
		s_texBlur = bgfx::createUniform("s_texBlur", bgfx::UniformType::Sampler);
		u_mtx = bgfx::createUniform("u_mtx", bgfx::UniformType::Mat4);
		u_tonemap = bgfx::createUniform("u_tonemap", bgfx::UniformType::Vec4);
		u_offset = bgfx::createUniform("u_offset", bgfx::UniformType::Vec4, 16);

		m_mesh = reousceModlue.LoadResource<OMesh>("meshes/bunny.bin");

		m_fbh.idx = bgfx::kInvalidHandle;

		m_lum[0] = bgfx::createFrameBuffer(128, 128, bgfx::TextureFormat::BGRA8);
		m_lum[1] = bgfx::createFrameBuffer(64, 64, bgfx::TextureFormat::BGRA8);
		m_lum[2] = bgfx::createFrameBuffer(16, 16, bgfx::TextureFormat::BGRA8);
		m_lum[3] = bgfx::createFrameBuffer(4, 4, bgfx::TextureFormat::BGRA8);
		m_lum[4] = bgfx::createFrameBuffer(1, 1, bgfx::TextureFormat::BGRA8);

		m_bright = bgfx::createFrameBuffer(bgfx::BackbufferRatio::Half, bgfx::TextureFormat::BGRA8);
		m_blur = bgfx::createFrameBuffer(bgfx::BackbufferRatio::Eighth, bgfx::TextureFormat::BGRA8);

		m_lumBgra8 = 0;
		if ((BGFX_CAPS_TEXTURE_BLIT | BGFX_CAPS_TEXTURE_READ_BACK) == (bgfx::getCaps()->supported & (BGFX_CAPS_TEXTURE_BLIT | BGFX_CAPS_TEXTURE_READ_BACK)))
		{
			m_rb = bgfx::createTexture2D(1, 1, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_READ_BACK);
		}
		else
		{
			m_rb.idx = bgfx::kInvalidHandle;
		}


		m_caps = bgfx::getCaps();
		m_oldWidth = 0;
		m_oldHeight = 0;
		m_oldReset = GProcessWindow::Get()._reset;

		_settings.m_speed = 0.37f;
		_settings.m_middleGray = 0.18f;
		_settings.m_white = 1.1f;
		_settings.m_threshold = 1.5f;

		m_scrollArea = 0;

		m_time = 0.0f;
	}

	void Update()
	{
		GProcessWindow& processWindow = GProcessWindow::Get();
		{
			if (!bgfx::isValid(m_fbh)
				|| m_oldWidth != processWindow._width
				|| m_oldHeight != processWindow._height
				|| m_oldReset != processWindow._reset)
			{
				// Recreate variable size render targets when resolution changes.
				m_oldWidth = processWindow._width;
				m_oldHeight = processWindow._height;
				m_oldReset = processWindow._reset;

				uint32_t msaa = (processWindow._reset&BGFX_RESET_MSAA_MASK) >> BGFX_RESET_MSAA_SHIFT;

				if (bgfx::isValid(m_fbh))
				{
					bgfx::destroy(m_fbh);
				}

				m_fbtextures[0] = bgfx::createTexture2D(
					uint16_t(processWindow._width)
					, uint16_t(processWindow._height)
					, false
					, 1
					, bgfx::TextureFormat::BGRA8
					, (uint64_t(msaa + 1) << BGFX_TEXTURE_RT_MSAA_SHIFT) | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP
				);

				const uint64_t textureFlags = BGFX_TEXTURE_RT_WRITE_ONLY | (uint64_t(msaa + 1) << BGFX_TEXTURE_RT_MSAA_SHIFT);

				bgfx::TextureFormat::Enum depthFormat =
					bgfx::isTextureValid(0, false, 1, bgfx::TextureFormat::D16, textureFlags) ? bgfx::TextureFormat::D16
					: bgfx::isTextureValid(0, false, 1, bgfx::TextureFormat::D24S8, textureFlags) ? bgfx::TextureFormat::D24S8
					: bgfx::TextureFormat::D32
					;

				m_fbtextures[1] = bgfx::createTexture2D(
					uint16_t(processWindow._width)
					, uint16_t(processWindow._height)
					, false
					, 1
					, depthFormat
					, textureFlags
				);

				m_fbh = bgfx::createFrameBuffer(BX_COUNTOF(m_fbtextures), m_fbtextures, true);
			}


			// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to view 0.
			bgfx::touch(0);

			int64_t now = bx::getHPCounter();
			static int64_t last = now;
			const int64_t frameTime = now - last;
			last = now;
			const double freq = double(bx::getHPFrequency());

			m_time += (float)(frameTime*_settings.m_speed / freq);

			bgfx::ViewId shuffle[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			bx::shuffle(&m_rng, shuffle, BX_COUNTOF(shuffle));

			bgfx::ViewId hdrSkybox = shuffle[0];
			bgfx::ViewId hdrMesh = shuffle[1];
			bgfx::ViewId hdrLuminance = shuffle[2];
			bgfx::ViewId hdrLumScale0 = shuffle[3];
			bgfx::ViewId hdrLumScale1 = shuffle[4];
			bgfx::ViewId hdrLumScale2 = shuffle[5];
			bgfx::ViewId hdrLumScale3 = shuffle[6];
			bgfx::ViewId hdrBrightness = shuffle[7];
			bgfx::ViewId hdrVBlur = shuffle[8];
			bgfx::ViewId hdrHBlurTonemap = shuffle[9];

			// Set views.
			bgfx::setViewName(hdrSkybox, "Skybox");
			bgfx::setViewClear(hdrSkybox, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
			bgfx::setViewRect(hdrSkybox, 0, 0, bgfx::BackbufferRatio::Equal);
			bgfx::setViewFrameBuffer(hdrSkybox, m_fbh);

			bgfx::setViewName(hdrMesh, "Mesh");
			bgfx::setViewClear(hdrMesh, BGFX_CLEAR_DISCARD_DEPTH | BGFX_CLEAR_DISCARD_STENCIL);
			bgfx::setViewRect(hdrMesh, 0, 0, bgfx::BackbufferRatio::Equal);
			bgfx::setViewFrameBuffer(hdrMesh, m_fbh);

			bgfx::setViewName(hdrLuminance, "Luminance");
			bgfx::setViewRect(hdrLuminance, 0, 0, 128, 128);
			bgfx::setViewFrameBuffer(hdrLuminance, m_lum[0]);

			bgfx::setViewName(hdrLumScale0, "Downscale luminance 0");
			bgfx::setViewRect(hdrLumScale0, 0, 0, 64, 64);
			bgfx::setViewFrameBuffer(hdrLumScale0, m_lum[1]);

			bgfx::setViewName(hdrLumScale1, "Downscale luminance 1");
			bgfx::setViewRect(hdrLumScale1, 0, 0, 16, 16);
			bgfx::setViewFrameBuffer(hdrLumScale1, m_lum[2]);

			bgfx::setViewName(hdrLumScale2, "Downscale luminance 2");
			bgfx::setViewRect(hdrLumScale2, 0, 0, 4, 4);
			bgfx::setViewFrameBuffer(hdrLumScale2, m_lum[3]);

			bgfx::setViewName(hdrLumScale3, "Downscale luminance 3");
			bgfx::setViewRect(hdrLumScale3, 0, 0, 1, 1);
			bgfx::setViewFrameBuffer(hdrLumScale3, m_lum[4]);

			bgfx::setViewName(hdrBrightness, "Brightness");
			bgfx::setViewRect(hdrBrightness, 0, 0, bgfx::BackbufferRatio::Half);
			bgfx::setViewFrameBuffer(hdrBrightness, m_bright);

			bgfx::setViewName(hdrVBlur, "Blur vertical");
			bgfx::setViewRect(hdrVBlur, 0, 0, bgfx::BackbufferRatio::Eighth);
			bgfx::setViewFrameBuffer(hdrVBlur, m_blur);

			bgfx::setViewName(hdrHBlurTonemap, "Blur horizontal + tonemap");
			bgfx::setViewRect(hdrHBlurTonemap, 0, 0, bgfx::BackbufferRatio::Equal);
			bgfx::FrameBufferHandle invalid = BGFX_INVALID_HANDLE;
			bgfx::setViewFrameBuffer(hdrHBlurTonemap, invalid);

			const bgfx::Caps* caps = bgfx::getCaps();
			float proj[16];
			bx::mtxOrtho(proj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0f, caps->homogeneousDepth);

			bgfx::ViewId order[] =
			{
				hdrSkybox,
				hdrMesh,
				hdrLuminance,
				hdrLumScale0,
				hdrLumScale1,
				hdrLumScale2,
				hdrLumScale3,
				hdrBrightness,
				hdrVBlur,
				hdrHBlurTonemap
			};
			bgfx::setViewOrder(0, BX_COUNTOF(order), order);

			// Set view and projection matrix for view 0.
			for (uint8_t ii = 0; ii < BX_COUNTOF(order); ++ii)
			{
				bgfx::setViewTransform(ii, NULL, proj);
			}

			const bx::Vec3 at = { 0.0f, 1.0f,  0.0f };
			const bx::Vec3 eye = { 0.0f, 1.0f, -2.5f };

			float mtx[16];
			bx::mtxRotateXY(mtx
				, 0.0f
				, m_time
			);

			const bx::Vec3 tmp = bx::mul(eye, mtx);

			float view[16];
			bx::mtxLookAt(view, tmp, at);
			bx::mtxProj(proj, 60.0f, float(processWindow._width) / float(processWindow._height), 0.1f, 100.0f, caps->homogeneousDepth);

			// Set view and projection matrix for view hdrMesh.
			bgfx::setViewTransform(hdrMesh, view, proj);

			float tonemap[4] = { _settings.m_middleGray, bx::square(_settings.m_white), _settings.m_threshold, m_time };

			// Render skybox into view hdrSkybox.
			bgfx::setTexture(0, s_texCube, m_uffizi);
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
			bgfx::setUniform(u_mtx, mtx);
			screenSpaceQuad((float)processWindow._width, (float)processWindow._height, true);
			bgfx::submit(hdrSkybox, m_skyProgram.GetProgram());

			// Render m_mesh into view hdrMesh.
			bgfx::setTexture(0, s_texCube, m_uffizi);
			bgfx::setUniform(u_tonemap, tonemap);
			m_mesh->submit(hdrMesh, m_meshProgram.GetProgram(), NULL);

			// Calculate luminance.
			setOffsets2x2Lum(u_offset, 128, 128);
			bgfx::setTexture(0, s_texColor, m_fbtextures[0]);
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
			screenSpaceQuad(128.0f, 128.0f, m_caps->originBottomLeft);
			bgfx::submit(hdrLuminance, m_lumProgram.GetProgram());

			// Downscale luminance 0.
			setOffsets4x4Lum(u_offset, 128, 128);
			bgfx::setTexture(0, s_texColor, bgfx::getTexture(m_lum[0]));
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
			screenSpaceQuad(64.0f, 64.0f, m_caps->originBottomLeft);
			bgfx::submit(hdrLumScale0, m_lumAvgProgram.GetProgram());

			// Downscale luminance 1.
			setOffsets4x4Lum(u_offset, 64, 64);
			bgfx::setTexture(0, s_texColor, bgfx::getTexture(m_lum[1]));
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
			screenSpaceQuad(16.0f, 16.0f, m_caps->originBottomLeft);
			bgfx::submit(hdrLumScale1, m_lumAvgProgram.GetProgram());

			// Downscale luminance 2.
			setOffsets4x4Lum(u_offset, 16, 16);
			bgfx::setTexture(0, s_texColor, bgfx::getTexture(m_lum[2]));
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
			screenSpaceQuad(4.0f, 4.0f, m_caps->originBottomLeft);
			bgfx::submit(hdrLumScale2, m_lumAvgProgram.GetProgram());

			// Downscale luminance 3.
			setOffsets4x4Lum(u_offset, 4, 4);
			bgfx::setTexture(0, s_texColor, bgfx::getTexture(m_lum[3]));
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
			screenSpaceQuad(1.0f, 1.0f, m_caps->originBottomLeft);
			bgfx::submit(hdrLumScale3, m_lumAvgProgram.GetProgram());

			// m_bright pass m_threshold is tonemap[3].
			setOffsets4x4Lum(u_offset, processWindow._width / 2, processWindow._height / 2);
			bgfx::setTexture(0, s_texColor, m_fbtextures[0]);
			bgfx::setTexture(1, s_texLum, bgfx::getTexture(m_lum[4]));
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
			bgfx::setUniform(u_tonemap, tonemap);
			screenSpaceQuad((float)processWindow._width / 2.0f, (float)processWindow._height / 2.0f, m_caps->originBottomLeft);
			bgfx::submit(hdrBrightness, m_brightProgram.GetProgram());

			// m_blur m_bright pass vertically.
			bgfx::setTexture(0, s_texColor, bgfx::getTexture(m_bright));
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
			bgfx::setUniform(u_tonemap, tonemap);
			screenSpaceQuad((float)processWindow._width / 8.0f, (float)processWindow._height / 8.0f, m_caps->originBottomLeft);
			bgfx::submit(hdrVBlur, m_blurProgram.GetProgram());

			// m_blur m_bright pass horizontally, do tonemaping and combine.
			bgfx::setTexture(0, s_texColor, m_fbtextures[0]);
			bgfx::setTexture(1, s_texLum, bgfx::getTexture(m_lum[4]));
			bgfx::setTexture(2, s_texBlur, bgfx::getTexture(m_blur));
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
			screenSpaceQuad((float)processWindow._width, (float)processWindow._height, m_caps->originBottomLeft);
			bgfx::submit(hdrHBlurTonemap, m_tonemapProgram.GetProgram());

			if (bgfx::isValid(m_rb))
			{
				bgfx::blit(hdrHBlurTonemap, m_rb, 0, 0, bgfx::getTexture(m_lum[4]));
				bgfx::readTexture(m_rb, &m_lumBgra8);
			}

			// Advance to next frame. Rendering thread will be kicked to
			// process submitted rendering primitives.
			bgfx::frame();
		}
	}
	struct Programs
	{
		void Init()
		{
		}
		void Destroy()
		{

		}
		bgfx::ProgramHandle m_skyProgram;
		bgfx::ProgramHandle m_lumProgram;
		bgfx::ProgramHandle m_lumAvgProgram;
		bgfx::ProgramHandle m_blurProgram;
		bgfx::ProgramHandle m_brightProgram;
		bgfx::ProgramHandle m_meshProgram;
		bgfx::ProgramHandle m_tonemapProgram;
	};
	struct Uniforms
	{
		void init()
		{
		}

		void submit()
		{
		}

		void destroy()
		{
		}
	};
	Programs _programs;
	Uniforms _uniforms;
	HDRSettings _settings;


	MouseState m_mouseState;

	FShaderProgram m_skyProgram;
	FShaderProgram m_lumProgram;
	FShaderProgram m_lumAvgProgram;
	FShaderProgram m_blurProgram;
	FShaderProgram m_brightProgram;
	FShaderProgram m_meshProgram;
	FShaderProgram m_tonemapProgram;

	bgfx::TextureHandle m_uffizi;
	bgfx::UniformHandle s_texCube;
	bgfx::UniformHandle s_texColor;
	bgfx::UniformHandle s_texLum;
	bgfx::UniformHandle s_texBlur;
	bgfx::UniformHandle u_mtx;
	bgfx::UniformHandle u_tonemap;
	bgfx::UniformHandle u_offset;

	OMesh* m_mesh;

	bgfx::TextureHandle m_fbtextures[2];
	bgfx::TextureHandle m_rb;
	bgfx::FrameBufferHandle m_fbh;
	bgfx::FrameBufferHandle m_lum[5];
	bgfx::FrameBufferHandle m_bright;
	bgfx::FrameBufferHandle m_blur;

	bx::RngMwc m_rng;

	uint32_t m_lumBgra8;

	uint32_t m_oldWidth;
	uint32_t m_oldHeight;
	uint32_t m_oldReset;



	int32_t m_scrollArea;

	const bgfx::Caps* m_caps;
	float m_time;
};

}