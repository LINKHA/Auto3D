#include "Renderer/IBLPipline.h"

namespace Auto3D
{

FIBLPipline::FIBLPipline()
{

}
FIBLPipline::~FIBLPipline()
{

}
void FIBLPipline::Init()
{
	_uniforms.init();
	m_programMesh.AttachShader("vs_ibl_mesh", "fs_ibl_mesh");

	GResourceModule& resourceModule = GResourceModule::Get();

	m_meshBunny = resourceModule.LoadResource<OMesh>("Meshes/bunny.bin");
	m_meshOrb = resourceModule.LoadResource<OMesh>("Meshes/orb.bin");
}

void FIBLPipline::Update(ACameraComponent* camera, ASkyboxComponent* skybox)
{
	if (camera == nullptr || skybox == nullptr)
		return;

	GProcessWindow& processWindow = GProcessWindow::Get();

	TMatrix4x4F projectionMatrix = camera->GetProjectionMatrix();
	TMatrix4x4F transposeViewMatrix = camera->GetViewMatrix().ToMatrix4().Transpose();
	TVector3F position = camera->GetOwner()->GetTransform()->GetPosition();

	_uniforms._glossiness = _settings._glossiness;
	_uniforms._reflectivity = _settings._reflectivity;
	_uniforms._exposure = _settings._exposure;
	_uniforms._metalOrSpec = float(_settings._metalOrSpec);
	_uniforms._doDiffuse = float(_settings._doDiffuse);
	_uniforms._doSpecular = float(_settings._doSpecular);
	_uniforms._doDiffuseIbl = float(_settings._doDiffuseIbl);
	_uniforms._doSpecularIbl = float(_settings._doSpecularIbl);
	bx::memCopy(_uniforms._rgbDiff, _settings._rgbDiff, 3 * sizeof(float));
	bx::memCopy(_uniforms._rgbSpec, _settings._rgbSpec, 3 * sizeof(float));
	bx::memCopy(_uniforms._lightDirection, _settings._lightDirection, 3 * sizeof(float));
	bx::memCopy(_uniforms._lightColor, _settings._lightColor, 3 * sizeof(float));

	_uniforms._cameraPos[0] = position._x;
	_uniforms._cameraPos[1] = position._y;
	_uniforms._cameraPos[2] = position._z;

	bgfx::setViewTransform(1, transposeViewMatrix.Data(), projectionMatrix.Data());
	bgfx::setViewRect(1, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));

	// Env mtx.
	TMatrix4x4F environmentViewMatrix = camera->GetEnvironmentViewMatrix();

	// Submit view.
	bx::memCopy(_uniforms._environmentViewMatrix, environmentViewMatrix.Data(), 16 * sizeof(float)); // Used for IBL.
	if (0 == _settings._meshSelection)
	{
		// Submit bunny.
		float mtx[16];
		bx::mtxSRT(mtx, 1.0f, 1.0f, 1.0f, 0.0f, bx::kPi, 0.0f, 0.0f, -0.80f, 0.0f);
		_uniforms._texture = skybox->GetTexture()->GetTextureHandle();
		_uniforms._textureIrrance = skybox->GetIrranceTexture()->GetTextureHandle();

		_uniforms.submit();
		m_meshBunny->submit(1, m_programMesh.GetProgram(), mtx);
	}
	else
	{
		// Submit orbs.
		for (float yy = 0, yend = 5.0f; yy < yend; yy += 1.0f)
		{
			for (float xx = 0, xend = 5.0f; xx < xend; xx += 1.0f)
			{
				const float scale = 1.2f;
				const float spacing = 2.2f;
				const float yAdj = -0.8f;

				float mtx[16];
				bx::mtxSRT(mtx
					, scale / xend
					, scale / xend
					, scale / xend
					, 0.0f
					, 0.0f
					, 0.0f
					, 0.0f + (xx / xend)*spacing - (1.0f + (scale - 1.0f)*0.5f - 1.0f / xend)
					, yAdj / yend + (yy / yend)*spacing - (1.0f + (scale - 1.0f)*0.5f - 1.0f / yend)
					, 0.0f
				);

				_uniforms._glossiness = xx * (1.0f / xend);
				_uniforms._reflectivity = (yend - yy)*(1.0f / yend);
				_uniforms._metalOrSpec = 0.0f;
				_uniforms._texture = skybox->GetTexture()->GetTextureHandle();
				_uniforms._textureIrrance = skybox->GetIrranceTexture()->GetTextureHandle();

				_uniforms.submit();
				m_meshOrb->submit(1, m_programMesh.GetProgram(), mtx);
			}
		}
	}
}

}
