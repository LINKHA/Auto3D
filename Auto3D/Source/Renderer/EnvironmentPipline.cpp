#include "Renderer/EnvironmentPipline.h"
#include "Renderer/ViewPass.h"

namespace Auto3D
{

void FEnvironmentPipline::Init()
{
	_equirectangularToCubemap.AttachShader("vs_cube_map", "fs_equirectangular_to_cubemap");
	_irradianceConvolution.AttachShader("vs_cube_map", "fs_irradiance_convolution");
	_prefilter.AttachShader("vs_cube_map", "fs_prefilter");
	_background.AttachShader("vs_background", "fs_background");

	_uniforms.Init();
}

void FEnvironmentPipline::Update(ACameraComponent* camera, ASkyboxComponent* skybox)
{
	if (camera == nullptr || skybox == nullptr)
		return;
	//bgfx::touch(0);
	GProcessWindow& processWindow = GProcessWindow::Get();

	switch (skybox->GetSkyboxType())
	{
	case ESkyboxType::HDR:
		{
			float view[16];
			bx::mtxIdentity(view);

			const bgfx::Caps* caps = bgfx::getCaps();
			bgfx::ViewId viewId = RENDERVIEW_SKYBOX_ID;
			//RENDERVIEW_SKYBOX_ID
			float proj[16];
			bx::mtxOrtho(proj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0, caps->homogeneousDepth);
			bgfx::setViewTransform(viewId, view, proj);
			bgfx::setViewFrameBuffer(viewId, BGFX_INVALID_HANDLE);
			bgfx::setViewRect(viewId, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
			screenSpaceQuad((float)processWindow._width, (float)processWindow._height, true);

			bgfx::setTexture(0, _uniforms.us_equirectangulaCubeMap, _uniforms._environmentViewTextureCube);
			bgfx::setUniform(_uniforms.u_environmentViewMatrix, _uniforms._environmentViewMatrix.Data());

			bgfx::submit(viewId, _background.GetProgram());
		}
		break;

	case ESkyboxType::IBL:
		{
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
			_uniforms._texture = skybox->GetIBLTexture()->GetTextureHandle();
			_uniforms._textureIrrance = skybox->GetIBLIrranceTexture()->GetTextureHandle();

			_uniforms.Submit();
			bgfx::submit(RENDERVIEW_SKYBOX_ID, skybox->GetShaderProgram().GetProgram());
		}
		break;
	default:
		break;
	}

}

void FEnvironmentPipline::CreatePrefilteIrradiance(ACameraComponent* camera, ASkyboxComponent* skybox)
{
	switch (skybox->GetSkyboxType())
	{
	case ESkyboxType::IBL:
		_uniforms._prefilterTextureCube = skybox->GetIBLTexture()->GetTextureHandle();
		_uniforms._textureIrrance = skybox->GetIBLIrranceTexture()->GetTextureHandle();
		/*for (int i = 0; i < 6; ++i)
		{
			bgfx::blit(ViewIdTemp(), _uniforms._environmentViewTextureCube, 0,
				0, 0, i,
				_uniforms._prefilterTextureCube, 0,
				0, 0, i,
				512, 512);
		}*/
	
		return;
		break;
	default:
		break;
	}

	GProcessWindow& processWindow = GProcessWindow::Get();

	TMatrix4x4F environmentViewMatrix = camera->GetEnvironmentViewMatrix();
	_uniforms._environmentViewMatrix = environmentViewMatrix.Transpose();
	_uniforms._environmentViewTexture = skybox->GetHDRTexture()->GetTextureHandle();

	static TMatrix4x4F captureProjection = Perspective(90.0f, 1.0f, 0.1f, 10.0f);
	static TMatrix4x4F captureViews[] =
	{
		LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(1.0f,  0.0f,  0.0f), TVector3F(0.0f, -1.0f,  0.0f)),
		LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(-1.0f,  0.0f,  0.0f), TVector3F(0.0f, -1.0f,  0.0f)),
		LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f,  1.0f,  0.0f), TVector3F(0.0f,  0.0f,  1.0f)),
		LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f, -1.0f,  0.0f), TVector3F(0.0f,  0.0f, -1.0f)),
		LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f,  0.0f,  1.0f), TVector3F(0.0f, -1.0f,  0.0f)),
		LookAt(TVector3F(0.0f, 0.0f, 0.0f), TVector3F(0.0f,  0.0f, -1.0f), TVector3F(0.0f, -1.0f,  0.0f))
	};

	static OMesh* cubeMesh = GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin");
	static FGeometry* cubeGeometry = cubeMesh->CreateGeometry();


	static bool rewrite = true;

	if (rewrite)
	{
		rewrite = false;

		for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._environmentViewTextureCubeFaceFb); ++ii)
		{
			//bgfx::ViewId viewId = bgfx::ViewId(ii + 1);
			bgfx::ViewId viewId = ViewIdConst();
			bgfx::setViewFrameBuffer(viewId, _uniforms._environmentViewTextureCubeFaceFb[ii]);
			bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR);
			bgfx::setViewRect(viewId, 0, 0, 512, 512);
			bgfx::setViewTransform(viewId, captureViews[ii].Data(), captureProjection.Data());

			bgfx::setTexture(0, _uniforms.us_equirectangularMap, _uniforms._environmentViewTexture);

			Submit(cubeGeometry, viewId, NULL, _equirectangularToCubemap.GetProgram());
		}
		bgfx::blit(ViewIdTemp(), _uniforms._environmentTempTexture[0], 0,
			0, 0, 0,
			_uniforms._environmentViewTextureCube, 0,
			0, 0, 3,
			512, 512);

		bgfx::blit(ViewIdTemp(), _uniforms._environmentTempTexture[1], 0,
			0, 0, 0,
			_uniforms._environmentViewTextureCube, 0,
			0, 0, 2,
			512, 512);

		bgfx::blit(ViewIdTemp(), _uniforms._environmentViewTextureCube, 0,
			0, 0, 3,
			_uniforms._environmentTempTexture[1], 0,
			0, 0, 0,
			512, 512);

		bgfx::blit(ViewIdTemp(), _uniforms._environmentViewTextureCube, 0,
			0, 0, 2,
			_uniforms._environmentTempTexture[0], 0,
			0, 0, 0,
			512, 512);

		/*for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._environmentViewTextureCubeFaceFb); ++ii)
		{
			bgfx::destroy(_uniforms._environmentViewTextureCubeFaceFb[ii]);
		}

		*/

		/*bgfx::destroy(_uniforms._environmentTempTexture[0]);
		bgfx::destroy(_uniforms._environmentTempTexture[1]);*/


		for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._irradianceViewTextureCubeFaceFb); ++ii)
		{
			bgfx::ViewId viewId = ViewIdConst();
			bgfx::setViewFrameBuffer(viewId, _uniforms._irradianceViewTextureCubeFaceFb[ii]);
			bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR);
			bgfx::setViewRect(viewId, 0, 0, 32, 32);
			bgfx::setViewTransform(viewId, captureViews[ii].Data(), captureProjection.Data());

			bgfx::setTexture(0, _uniforms.us_equirectangulaCubeMap, _uniforms._environmentViewTextureCube);
			Submit(cubeGeometry, viewId, NULL, _irradianceConvolution.GetProgram());
		}


		for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._prefilterTextureCubeFaceFb); ++ii)
		{
			unsigned int mip = ii / 6;
			unsigned int layer = ii % 6;
			unsigned int mipSize = 128 * std::pow(0.5, mip);

			bgfx::ViewId viewId = ViewIdConst();
			bgfx::setViewFrameBuffer(viewId, _uniforms._prefilterTextureCubeFaceFb[ii]);

			bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
			bgfx::setViewRect(viewId, 0, 0, mipSize, mipSize);
			bgfx::setViewTransform(viewId, captureViews[layer].Data(), captureProjection.Data());

			bgfx::setTexture(0, _uniforms.us_equirectangulaCubeMap, _uniforms._environmentViewTextureCube);
			float roughness = (float)mip / (float)(5 - 1);
			bgfx::setUniform(_uniforms.u_roughness, TVector4F(roughness, 0.0f, 0.0f, 0.0f).Data());

			Submit(cubeGeometry, viewId, NULL, _prefilter.GetProgram());
		}

		for (uint32_t ii = 0; ii < BX_COUNTOF(_uniforms._prefilterTextureCubeFaceFb); ++ii)
		{
			unsigned int mip = ii / 6;
			unsigned int layer = ii % 6;
			unsigned int mipSize = 128 * std::pow(0.5, mip);

			bgfx::blit(ViewIdTemp(), _uniforms._prefilterTextureCube, mip,
				0, 0, layer,
				_uniforms._prefilterTextureFbCube[mip], 0,
				0, 0, layer,
				mipSize, mipSize);
		}

	}

	
}


}