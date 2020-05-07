#pragma once
#include "AutoConfig.h"
#include "RHI/bgfx_utils.h"
#include "Renderer/RendererDef.h"
#include "Resource/ResourceCache.h"
#include "Platform/ProcessWindow.h"
#include "Resource/Mesh.h"
#include "Component/CameraComponent.h"
#include "Component/SkyboxComponent.h"
#include "Renderer/Batch.h"

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/rng.h>
#include <bx/timer.h>

namespace Auto3D
{


void sSubmitTemp(FGeometry* geometry, bgfx::ViewId id, bgfx::ProgramHandle program, const float* mtx, uint64_t state = BGFX_STATE_MASK);
	

struct HDRSettings
{
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


		_programs.Init();

		u_environmentViewMatrix = bgfx::createUniform("u_environmentViewMatrix", bgfx::UniformType::Mat4);
		s_texCube = bgfx::createUniform("s_texCube", bgfx::UniformType::Sampler);
		s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
		s_texLum = bgfx::createUniform("s_texLum", bgfx::UniformType::Sampler);
		s_texBlur = bgfx::createUniform("s_texBlur", bgfx::UniformType::Sampler);
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

		_settings.m_middleGray = 0.18f;
		_settings.m_white = 1.1f;
		_settings.m_threshold = 1.5f;

		m_scrollArea = 0;
	}

	void Update(ACameraComponent* camera, ASkyboxComponent* skybox)
	{
		if (camera == nullptr || skybox == nullptr)
			return;

		GProcessWindow& processWindow = GProcessWindow::Get();

		TMatrix4x4F projectionMatrix = camera->GetProjectionMatrix();
		TMatrix4x4F transposeViewMatrix = camera->GetViewMatrix().ToMatrix4().Transpose();
		TMatrix4x4F environmentViewMatrix = camera->GetEnvironmentViewMatrix();

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

		bgfx::ViewId shuffle[10] = { 
			 RENDERVIEW_hdrSkybox,
			 RENDERVIEW_hdrMesh	,
			 RENDERVIEW_hdrLuminance,
			 RENDERVIEW_hdrLumScale0,
			 RENDERVIEW_hdrLumScale1,
			 RENDERVIEW_hdrLumScale2,
			 RENDERVIEW_hdrLumScale3,
			 RENDERVIEW_hdrBrightness,
			 RENDERVIEW_hdrVBlur,
			 RENDERVIEW_hdrHBlurTonemap,
		};
		bx::shuffle(&m_rng, shuffle, BX_COUNTOF(shuffle));

		hdrSkybox = shuffle[0];
		hdrMesh = shuffle[1];
		hdrLuminance = shuffle[2];
		hdrLumScale0 = shuffle[3];
		hdrLumScale1 = shuffle[4];
		hdrLumScale2 = shuffle[5];
		hdrLumScale3 = shuffle[6];
		hdrBrightness = shuffle[7];
		hdrVBlur = shuffle[8];
		hdrHBlurTonemap = shuffle[9];

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
		bgfx::setViewOrder(RENDERVIEW_hdrSkybox, BX_COUNTOF(order), order);


		// Set view and projection matrix for view 0.
		for (uint8_t ii = 0; ii < BX_COUNTOF(shuffle); ++ii)
		{
			bgfx::setViewTransform(RENDERVIEW_hdrSkybox + ii, NULL, proj);
		}

		// Set view and projection matrix for view hdrMesh.
		bgfx::setViewTransform(hdrMesh, transposeViewMatrix.Data(), projectionMatrix.Data());

		float tonemap[4] = { _settings.m_middleGray, bx::square(_settings.m_white), _settings.m_threshold, 0.0f };

		// Render skybox into view hdrSkybox.
		bgfx::setTexture(0, s_texCube, skybox->GetHDRTexture()->GetTextureHandle());
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		bgfx::setUniform(u_environmentViewMatrix, environmentViewMatrix.Data());
		screenSpaceQuad((float)processWindow._width, (float)processWindow._height, true);
		bgfx::submit(hdrSkybox, _programs.m_skyProgram.GetProgram());



		// Calculate luminance.
		setOffsets2x2Lum(u_offset, 128, 128);
		bgfx::setTexture(0, s_texColor, m_fbtextures[0]);
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		screenSpaceQuad(128.0f, 128.0f, m_caps->originBottomLeft);
		bgfx::submit(hdrLuminance, _programs.m_lumProgram.GetProgram());

		// Downscale luminance 0.
		setOffsets4x4Lum(u_offset, 128, 128);
		bgfx::setTexture(0, s_texColor, bgfx::getTexture(m_lum[0]));
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		screenSpaceQuad(64.0f, 64.0f, m_caps->originBottomLeft);
		bgfx::submit(hdrLumScale0, _programs.m_lumAvgProgram.GetProgram());

		// Downscale luminance 1.
		setOffsets4x4Lum(u_offset, 64, 64);
		bgfx::setTexture(0, s_texColor, bgfx::getTexture(m_lum[1]));
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		screenSpaceQuad(16.0f, 16.0f, m_caps->originBottomLeft);
		bgfx::submit(hdrLumScale1, _programs.m_lumAvgProgram.GetProgram());

		// Downscale luminance 2.
		setOffsets4x4Lum(u_offset, 16, 16);
		bgfx::setTexture(0, s_texColor, bgfx::getTexture(m_lum[2]));
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		screenSpaceQuad(4.0f, 4.0f, m_caps->originBottomLeft);
		bgfx::submit(hdrLumScale2, _programs.m_lumAvgProgram.GetProgram());

		// Downscale luminance 3.
		setOffsets4x4Lum(u_offset, 4, 4);
		bgfx::setTexture(0, s_texColor, bgfx::getTexture(m_lum[3]));
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		screenSpaceQuad(1.0f, 1.0f, m_caps->originBottomLeft);
		bgfx::submit(hdrLumScale3, _programs.m_lumAvgProgram.GetProgram());

		// m_bright pass m_threshold is tonemap[3].
		setOffsets4x4Lum(u_offset, processWindow._width / 2, processWindow._height / 2);
		bgfx::setTexture(0, s_texColor, m_fbtextures[0]);
		bgfx::setTexture(1, s_texLum, bgfx::getTexture(m_lum[4]));
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		bgfx::setUniform(u_tonemap, tonemap);
		screenSpaceQuad((float)processWindow._width / 2.0f, (float)processWindow._height / 2.0f, m_caps->originBottomLeft);
		bgfx::submit(hdrBrightness, _programs.m_brightProgram.GetProgram());

		// m_blur m_bright pass vertically.
		bgfx::setTexture(0, s_texColor, bgfx::getTexture(m_bright));
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		bgfx::setUniform(u_tonemap, tonemap);
		screenSpaceQuad((float)processWindow._width / 8.0f, (float)processWindow._height / 8.0f, m_caps->originBottomLeft);
		bgfx::submit(hdrVBlur, _programs.m_blurProgram.GetProgram());

		// m_blur m_bright pass horizontally, do tonemaping and combine.
		bgfx::setTexture(0, s_texColor, m_fbtextures[0]);
		bgfx::setTexture(1, s_texLum, bgfx::getTexture(m_lum[4]));
		bgfx::setTexture(2, s_texBlur, bgfx::getTexture(m_blur));
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		screenSpaceQuad((float)processWindow._width, (float)processWindow._height, m_caps->originBottomLeft);
		bgfx::submit(hdrHBlurTonemap, _programs.m_tonemapProgram.GetProgram());

		if (bgfx::isValid(m_rb))
		{
			bgfx::blit(hdrHBlurTonemap, m_rb, 0, 0, bgfx::getTexture(m_lum[4]));
			bgfx::readTexture(m_rb, &m_lumBgra8);
		}
	}

	void RenderBatch(ACameraComponent* camera, ASkyboxComponent* skybox, TVector<FBatch>& batches)
	{
		float tonemap[4] = { _settings.m_middleGray, bx::square(_settings.m_white), _settings.m_threshold, 0.0f };
	
		for (auto bIt = batches.Begin(); bIt != batches.End();)
		{
			FBatch& batch = *bIt;
			bool instance = batch._type == EGeometryType::INSTANCED;
			int batchesAddCount = 0;
			FGeometry* geometry = batch._pass._geometry;
			OMaterial* material = batch._pass._material;
			TMatrix4x4F& modelMatrix = batch._pass._worldMatrix->ToMatrix4().Transpose();
			// Render m_mesh into view hdrMesh.
			bgfx::setTexture(0, s_texCube, skybox->GetHDRTexture()->GetTextureHandle());
			bgfx::setUniform(u_tonemap, tonemap);
			sSubmitTemp(geometry, hdrMesh, _programs.m_meshProgram.GetProgram(), modelMatrix.Data());
			batchesAddCount = 1;
			bIt += batchesAddCount;
		}
	}

	struct Programs
	{
		void Init()
		{
			m_skyProgram.AttachShader("vs_hdr_skybox", "fs_hdr_skybox");
			m_lumProgram.AttachShader("vs_hdr_lum", "fs_hdr_lum");
			m_lumAvgProgram.AttachShader("vs_hdr_lumavg", "fs_hdr_lumavg");
			m_blurProgram.AttachShader("vs_hdr_blur", "fs_hdr_blur");
			m_brightProgram.AttachShader("vs_hdr_bright", "fs_hdr_bright");
			m_meshProgram.AttachShader("vs_hdr_mesh", "fs_hdr_mesh");
			m_tonemapProgram.AttachShader("vs_hdr_tonemap", "fs_hdr_tonemap");
		}
		void Destroy()
		{

		}
		FShaderProgram m_skyProgram;
		FShaderProgram m_lumProgram;
		FShaderProgram m_lumAvgProgram;
		FShaderProgram m_blurProgram;
		FShaderProgram m_brightProgram;
		FShaderProgram m_meshProgram;
		FShaderProgram m_tonemapProgram;
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

	

	bgfx::UniformHandle u_environmentViewMatrix;
	bgfx::UniformHandle s_texCube;
	bgfx::UniformHandle s_texColor;
	bgfx::UniformHandle s_texLum;
	bgfx::UniformHandle s_texBlur;
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

	bgfx::ViewId hdrSkybox;
	bgfx::ViewId hdrMesh;
	bgfx::ViewId hdrLuminance;
	bgfx::ViewId hdrLumScale0;
	bgfx::ViewId hdrLumScale1;
	bgfx::ViewId hdrLumScale2;
	bgfx::ViewId hdrLumScale3;
	bgfx::ViewId hdrBrightness;
	bgfx::ViewId hdrVBlur;
	bgfx::ViewId hdrHBlurTonemap;
};

}