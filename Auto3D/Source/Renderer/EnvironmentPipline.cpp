#include "Renderer/EnvironmentPipline.h"

namespace Auto3D
{

void FEnvironmentPipline::Init()
{
	_uniforms.Init();
}

void FEnvironmentPipline::Update(ACameraComponent* camera, ASkyboxComponent* skybox)
{
	if (camera == nullptr || skybox == nullptr)
		return;
	//bgfx::touch(0);
	GProcessWindow& processWindow = GProcessWindow::Get();

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

}