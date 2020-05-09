#include "Renderer/IBLPipline.h"

#include <bgfx/bgfx.h>

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
	_program.AttachShader("vs_ibl_mesh", "fs_ibl_mesh");
	_programInstance.AttachShader("vs_ibl_mesh_i", "fs_ibl_mesh");

	GResourceModule& resourceModule = GResourceModule::Get();
}

void FIBLPipline::Update(ACameraComponent* camera, ASkyboxComponent* skybox, TVector<FBatch>& batches)
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

	bgfx::setViewTransform(RENDERVIEW_NO_LIGHT_IBL, transposeViewMatrix.Data(), projectionMatrix.Data());
	bgfx::setViewRect(RENDERVIEW_NO_LIGHT_IBL, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));

	// Env mtx.
	TMatrix4x4F environmentViewMatrix = camera->GetEnvironmentViewMatrix();

	// Submit view.
	bx::memCopy(_uniforms._environmentViewMatrix, environmentViewMatrix.Data(), 16 * sizeof(float)); // Used for IBL.
}

}
