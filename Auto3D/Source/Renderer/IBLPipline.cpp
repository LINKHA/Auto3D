#include "Renderer/IBLPipline.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{

void SubmitTemp(FGeometry* geometry,bgfx::ViewId id, bgfx::ProgramHandle program, const float* mtx, uint64_t state = BGFX_STATE_MASK)
{
	if (BGFX_STATE_MASK == state)
	{
		state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CCW
			| BGFX_STATE_MSAA
			;
	}

	bgfx::setTransform(mtx);
	bgfx::setState(state);

	TVector<TPair<FMeshGroup*, bgfx::OcclusionQueryHandle>>& geometryValue = geometry->_geometryValue;

	for (auto it = geometryValue.Begin(), itEnd = geometryValue.End(); it != itEnd; ++it)
	{
		FMeshGroup* group = it->_first;
		//bgfx::OcclusionQueryHandle& occlusionQuery = it->_second;

		bgfx::setIndexBuffer(group->_ibh);
		bgfx::setVertexBuffer(0, group->_vbh);
		bgfx::submit(id, program, 0, it != itEnd - 1);
	}
}

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

	//bgfx::setViewTransform(1, transposeViewMatrix.Data(), projectionMatrix.Data());
	//bgfx::setViewRect(1, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));

	// Env mtx.
	TMatrix4x4F environmentViewMatrix = camera->GetEnvironmentViewMatrix();

	// Submit view.
	bx::memCopy(_uniforms._environmentViewMatrix, environmentViewMatrix.Data(), 16 * sizeof(float)); // Used for IBL.
	
	/*for (auto bIt = batches.Begin(); bIt != batches.End();)
	{
		FBatch& batch = *bIt;
		bool instance = batch._type == EGeometryType::INSTANCED;
		int batchesAddCount = 0;

		FGeometry* geometry = batch._pass._geometry;
		OMaterial* material = batch._pass._material;
		TMatrix4x4F& modelMatrix = batch._pass._worldMatrix->ToMatrix4().Transpose();

		_uniforms._texture = skybox->GetIBLTexture()->GetTextureHandle();
		_uniforms._textureIrrance = skybox->GetIBLIrranceTexture()->GetTextureHandle();

		_uniforms.submit();
		SubmitTemp(geometry, 1, m_programMesh.GetProgram(), modelMatrix.Data());

		batchesAddCount = 1;
		bIt += batchesAddCount;
	}*/
}

}
