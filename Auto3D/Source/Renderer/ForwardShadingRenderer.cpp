#include "Renderer/ForwardShadingRenderer.h"
#include "Platform/PlatformDef.h"
#include "Platform/Args.h"

#include "Time/Time.h"
#include "Component/MeshComponent.h"
#include "Resource/Mesh.h"

#include "Renderer/Pass.h"
#include "Resource/Material.h"
#include "Resource/ResourceCache.h"

#include "Component/TransformComponent.h"
#include "Component/CameraComponent.h"
#include "Component/LightComponent.h"

#include "Gameplay/WorldContext.h"
#include "Gameplay/World.h"
#include "Platform/ProcessWindow.h"
#include "Math/Matrix3x4.h"
#include "Math/Matrix4x4.h"
#include "RHI/bgfx_utils.h"

#include "Resource/Material.h"
#include "Renderer/ShadowRenderer.h"
#include "Renderer/IBLPipline.h"
#include "Renderer/EnvironmentPipline.h"

namespace Auto3D
{
#define RENDER_SHADOW_PASS_ID 20
//#define RENDER_SCENE_PASS_ID  21
#define RENDER_OCCLUSION_PASS_ID 22

void SubmitShadowInstance(FGeometry* geometry, uint8_t _viewId, bgfx::InstanceDataBuffer* idb, bgfx::ProgramHandle _program, const FRenderState& _renderState, bgfx::TextureHandle _texture, bool _submitShadowMaps = false)
{
	TVector<TPair<FMeshGroup*, bgfx::OcclusionQueryHandle>>& geometryValue = geometry->_geometryValue;

	// Set textures.
	if (bgfx::kInvalidHandle != _texture.idx)
	{
		bgfx::setTexture(0, FShadowPipline::s_texColor, _texture);
	}

	if (_submitShadowMaps)
	{
		for (uint8_t ii = 0; ii < ShadowMapRenderTargets::Count; ++ii)
		{
			bgfx::setTexture(4 + ii, FShadowPipline::s_shadowMap[ii], bgfx::getTexture(FShadowPipline::s_rtShadowMap[ii]));
		}
	}

	// Apply render state.
	bgfx::setStencil(_renderState._fstencil, _renderState._bstencil);
	bgfx::setState(_renderState._state, _renderState._blendFactorRgba);
	bgfx::setInstanceDataBuffer(idb);
	// Set uniforms.
	FShadowPipline::Get().SubmitPerDrawUniforms();

	for (auto it = geometryValue.Begin(), itEnd = geometryValue.End(); it != itEnd; ++it)
	{
		FMeshGroup* group = it->_first;
		bgfx::OcclusionQueryHandle& occlusionQuery = it->_second;

		bgfx::setCondition(occlusionQuery, true);
		// Set model matrix for rendering.
		bgfx::setIndexBuffer(group->_ibh);
		bgfx::setVertexBuffer(0, group->_vbh);

		// Submit.
		bgfx::submit(_viewId, _program ,0, it != itEnd - 1);
	}
}


void SubmitShadow(FGeometry* geometry, uint8_t _viewId, float* _mtx, bgfx::ProgramHandle _program, const FRenderState& _renderState, bgfx::TextureHandle _texture, bool _submitShadowMaps = false)
{
	TVector<TPair<FMeshGroup*, bgfx::OcclusionQueryHandle>>& geometryValue = geometry->_geometryValue;

	// Set textures.
	if (bgfx::kInvalidHandle != _texture.idx)
	{
		bgfx::setTexture(0, FShadowPipline::s_texColor, _texture);
	}

	if (_submitShadowMaps)
	{
		for (uint8_t ii = 0; ii < ShadowMapRenderTargets::Count; ++ii)
		{
			bgfx::setTexture(4 + ii, FShadowPipline::s_shadowMap[ii], bgfx::getTexture(FShadowPipline::s_rtShadowMap[ii]));
		}
	}

	bgfx::setTransform(_mtx);
	// Apply render state.
	bgfx::setStencil(_renderState._fstencil, _renderState._bstencil);
	bgfx::setState(_renderState._state, _renderState._blendFactorRgba);
	

	// Set uniforms.
	FShadowPipline::Get().SubmitPerDrawUniforms();

	

	for (auto it = geometryValue.Begin(), itEnd = geometryValue.End(); it != itEnd; ++it)
	{
		FMeshGroup* group = it->_first;
		bgfx::OcclusionQueryHandle& occlusionQuery = it->_second;

		bgfx::setCondition(occlusionQuery, true);
		bgfx::setIndexBuffer(group->_ibh);
		bgfx::setVertexBuffer(0, group->_vbh);

		// Submit.
		bgfx::submit(_viewId, _program ,0, it != itEnd - 1);
	}
}

void SubmitOcclusion(FGeometry* geometry, uint8_t _viewId, float* _mtx, bgfx::ProgramHandle _program, const FRenderState& _renderState, bgfx::TextureHandle _texture, bool _submitShadowMaps = false)
{
	TVector<TPair<FMeshGroup*, bgfx::OcclusionQueryHandle>>& geometryValue = geometry->_geometryValue;

	// Set textures.
	if (bgfx::kInvalidHandle != _texture.idx)
	{
		bgfx::setTexture(0, FShadowPipline::s_texColor, _texture);
	}

	if (_submitShadowMaps)
	{
		for (uint8_t ii = 0; ii < ShadowMapRenderTargets::Count; ++ii)
		{
			bgfx::setTexture(4 + ii, FShadowPipline::s_shadowMap[ii], bgfx::getTexture(FShadowPipline::s_rtShadowMap[ii]));
		}
	}
	bgfx::setTransform(_mtx);
	// Apply render state.
	bgfx::setStencil(_renderState._fstencil, _renderState._bstencil);
	bgfx::setState(_renderState._state, _renderState._blendFactorRgba);

	// Set uniforms.
	FShadowPipline::Get().SubmitPerDrawUniforms();
	

	for (auto it = geometryValue.Begin(), itEnd = geometryValue.End(); it != itEnd; ++it)
	{
		FMeshGroup* group = it->_first;
		bgfx::OcclusionQueryHandle& occlusionQuery = it->_second;
		
		// Set model matrix for rendering.
		
		bgfx::setIndexBuffer(group->_ibh);
		bgfx::setVertexBuffer(0, group->_vbh);

		// Submit.
		bgfx::submit(_viewId, _program, occlusionQuery,0, it != itEnd - 1);
	}
}


void SubmitOcclusionInstace(FGeometry* geometry, uint8_t _viewId, bgfx::InstanceDataBuffer* idb, bgfx::ProgramHandle _program, const FRenderState& _renderState, bgfx::TextureHandle _texture, bool _submitShadowMaps = false)
{

	TVector<TPair<FMeshGroup*, bgfx::OcclusionQueryHandle>>& geometryValue = geometry->_geometryValue;

	// Set textures.
	if (bgfx::kInvalidHandle != _texture.idx)
	{
		bgfx::setTexture(0, FShadowPipline::s_texColor, _texture);
	}

	if (_submitShadowMaps)
	{
		for (uint8_t ii = 0; ii < ShadowMapRenderTargets::Count; ++ii)
		{
			bgfx::setTexture(4 + ii, FShadowPipline::s_shadowMap[ii], bgfx::getTexture(FShadowPipline::s_rtShadowMap[ii]));
		}
	}

	// Apply render state.
	bgfx::setStencil(_renderState._fstencil, _renderState._bstencil);
	bgfx::setState(_renderState._state, _renderState._blendFactorRgba);

	// Set uniforms.
	FShadowPipline::Get().SubmitPerDrawUniforms();


	for (auto it = geometryValue.Begin(), itEnd = geometryValue.End(); it != itEnd; ++it)
	{
		FMeshGroup* group = it->_first;
		bgfx::OcclusionQueryHandle& occlusionQuery = it->_second;
	
		// Set model matrix for rendering.
		bgfx::setInstanceDataBuffer(idb);
		bgfx::setIndexBuffer(group->_ibh);
		bgfx::setVertexBuffer(0, group->_vbh);

		// Submit.
		bgfx::submit(_viewId, _program, occlusionQuery, 0, it != itEnd - 1);
	}
}

void SubmitShadowInstance(FGeometry* geometry, uint8_t _viewId, bgfx::InstanceDataBuffer* idb, bgfx::ProgramHandle _program, const FRenderState& _renderState, bool _submitShadowMaps = false)
{
	bgfx::TextureHandle texture = BGFX_INVALID_HANDLE;
	SubmitShadowInstance(geometry, _viewId, idb, _program, _renderState, texture, _submitShadowMaps);
}

void SubmitShadow(FGeometry* geometry, uint8_t _viewId, float* _mtx, bgfx::ProgramHandle _program, const FRenderState& _renderState, bool _submitShadowMaps = false)
{
	bgfx::TextureHandle texture = BGFX_INVALID_HANDLE;
	SubmitShadow(geometry, _viewId, _mtx, _program, _renderState, texture, _submitShadowMaps);
}

void SubmitOcclusionInstace(FGeometry* geometry, uint8_t _viewId, bgfx::InstanceDataBuffer* idb, bgfx::ProgramHandle _program, const FRenderState& _renderState, bool _submitShadowMaps = false)
{
	bgfx::TextureHandle texture = BGFX_INVALID_HANDLE;
	SubmitOcclusionInstace(geometry, _viewId, idb, _program, _renderState, texture, _submitShadowMaps);
}

void SubmitOcclusion(FGeometry* geometry, uint8_t _viewId, float* _mtx, bgfx::ProgramHandle _program, const FRenderState& _renderState, bool _submitShadowMaps = false)
{
	bgfx::TextureHandle texture = BGFX_INVALID_HANDLE;
	SubmitOcclusion(geometry, _viewId, _mtx, _program, _renderState, texture, _submitShadowMaps);
}

FEnvironmentPipline FForwardShadingRenderer::_environmentPipline;
FIBLPipline FForwardShadingRenderer::_iblPipline;
FShadowPipline FForwardShadingRenderer::_shadowPipline;

FForwardShadingRenderer::FForwardShadingRenderer() :
	_backbufferSize(TVector2F(AUTO_DEFAULT_WIDTH,AUTO_DEFAULT_HEIGHT)),
	_debug(BGFX_DEBUG_NONE),
	_reset(BGFX_RESET_VSYNC),
	_backbufferColor(0.2f, 0.2f, 0.2f, 1),
	_depth(1.0f),
	_stencil(0),
	_invisibleBatch(0),
	_visibleBatch(0),
	_currentCamera(nullptr)
{

}

FForwardShadingRenderer::~FForwardShadingRenderer()
{

}

void FForwardShadingRenderer::Init()
{
	FArgs& args = FArgs::Get();
	_backbufferSize = TVector2F(AUTO_DEFAULT_WIDTH, AUTO_DEFAULT_HEIGHT);
	_debug = BGFX_DEBUG_NONE;
	_reset = 0
		| BGFX_RESET_VSYNC
		| BGFX_RESET_MSAA_X16;

	bgfx::Init init;
	init.type = args._type;
	init.vendorId = args._pciId;
	init.resolution.width = _backbufferSize._x;
	init.resolution.height = _backbufferSize._y;
	init.resolution.reset = _reset;
	bgfx::init(init);

	// Enable debug text.
	bgfx::setDebug(_debug);

	//// Set views  clear state.
	//bgfx::setViewClear(RENDER_SCENE_PASS_ID
	//	, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
	//	, _backbufferColor.ToUInt()
	//	, _depth
	//	, _stencil
	//);

	// Set views  clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
	);


	_programs.Init();

	FRendererDef().Init();

	FShadowPipline::Get().Init();
	_iblPipline.Init();
	_environmentPipline.Init();
}

void FForwardShadingRenderer::Render()
{
	PrepareView();
	// This dummy draw call is here to make sure that view 0 is cleared
	// if no other draw calls are submitted to view 0.
	//bgfx::touch(RENDER_SHADOW_PASS_ID);

	AWorld* world = FWorldContext::Get().GetActiveWorld();
	TVector<ACameraComponent*>& cameras = world->GetCameras();

	for (auto it = cameras.Begin(); it != cameras.End(); ++it)
	{
		PrepareView();
		CollectActors(world, *it);
		CollectBatch();

		ACameraComponent* camera = dynamic_cast<ACameraComponent*>(*it);
		_currentCamera = camera;

		// Set the view transform each camera is set once in the view
		{	
			// Set up matrices for gbuffer
			TMatrix3x4F viewMatrix = camera->GetViewMatrix();
			// Because the original location is not unified
			TMatrix4x4F transposeViewMatrix = viewMatrix.ToMatrix4().Transpose();

			camera->SetAspectRatio(float(_backbufferSize._x) / float(_backbufferSize._y));
			TMatrix4x4F projectionMatrix = camera->GetProjectionMatrix();

			// Ordinary pipeline view rect
			bgfx::setViewTransform(RENDER_OCCLUSION_PASS_ID, transposeViewMatrix.Data(), projectionMatrix.Data());
			bgfx::setViewRect(RENDER_OCCLUSION_PASS_ID, 0, 0, uint16_t(_backbufferSize._x), uint16_t(_backbufferSize._y));
		}

		RenderBatches();
	}
	
	// Advance to next frame. Rendering thread will be kicked to
	// process submitted rendering primitives.
	bgfx::frame();
}


void FForwardShadingRenderer::RenderBatches()
{
	AWorld* world = FWorldContext::Get().GetActiveWorld();
	ASkyboxComponent* skybox = world->GetSkybox();
	TVector<FBatch>& batches = _batchQueues._batches;

	_environmentPipline.Update(_currentCamera, skybox);

	_iblPipline.Update(_currentCamera, skybox, batches);


	for (auto lIt = _lightActor.Begin(); lIt != _lightActor.End(); ++lIt)
	{
		AActor* lightActor = *lIt;
		TVector<ALightComponent*>& lightComponents = lightActor->GetLightComponents();
		for (auto lcIt = lightComponents.Begin(); lcIt != lightComponents.End(); ++lcIt)
		{
			ALightComponent* lightComponent = *lcIt;

			TMatrix4x4F& lightProj = lightComponent->GetLightProj();
			TMatrix4x4F& lightView = lightComponent->GetLightView();
			TMatrix4x4F& mtxShadow = lightComponent->GetMtxShadow();
			TVector3F lightPosition = lightComponent->GetOwner()->GetTransform()->GetWorldPosition();

			FShadowPipline::Get().Update(_currentCamera, lightComponent);

			//TVector<FBatch>& batches = _batchQueues._batches;
			for (auto bIt = batches.Begin(); bIt != batches.End();)
			{
				FBatch& batch = *bIt;
				bool instance = batch._type == EGeometryType::INSTANCED;
				int batchesAddCount = 0;


				if (instance)
				{
					int instanceStart = batch._instanceStart;
					int instanceCount = batch._instanceCount;

					// stride = 64 bytes for 4x4 matrix
					const uint16_t instanceStride = sizeof(TMatrix4x4F);
					const uint32_t numInstances = instanceCount;

					bgfx::InstanceDataBuffer idb;
					bgfx::allocInstanceDataBuffer(&idb, numInstances, instanceStride);
					uint8_t* data = idb.data;

					TMatrix4x4F lightMtx;
					// Get model matrix.
					for (auto i = instanceStart; i < instanceStart + instanceCount; ++i)
					{
						TMatrix4x4F& modelMatrix = batches[i]._pass._worldMatrix->ToMatrix4().Transpose();

						memcpy(data, modelMatrix.Data(), instanceStride);
						data += instanceStride;
					}


					FGeometry* geometry = batch._pass._geometry;
					OMaterial* material = batch._pass._material;
					FShadowMapSettings* currentShadowMapSettings = &FShadowPipline::_shadowMapSettings[FShadowPipline::_shadowSceneSettings.m_lightType][FShadowPipline::_shadowSceneSettings.m_depthImpl][FShadowPipline::_shadowSceneSettings.m_smImpl];
					AttachShader(batch._pass);


					uint8_t drawNum;
					if (ELightType::SpotLight == FShadowPipline::_shadowSceneSettings.m_lightType)
					{
						drawNum = 1;
					}
					else if (ELightType::PointLight == FShadowPipline::_shadowSceneSettings.m_lightType)
					{
						drawNum = 4;
					}
					else //LightType::DirectionalLight == settings.m_lightType)
					{
						drawNum = uint8_t(FShadowPipline::_shadowSceneSettings.m_numSplits);
					}

					for (uint8_t ii = 0; ii < drawNum; ++ii)
					{
						const uint8_t viewId = RENDERVIEW_SHADOWMAP_1_ID + ii;

						uint8_t renderStateIndex = FRenderState::ShadowMap_PackDepth;
						if (ELightType::PointLight == FShadowPipline::_shadowSceneSettings.m_lightType && FShadowPipline::_shadowSceneSettings.m_stencilPack)
						{
							renderStateIndex = uint8_t((ii < 2) ? FRenderState::ShadowMap_PackDepthHoriz : FRenderState::ShadowMap_PackDepthVert);
						}

						// Floor.
						SubmitShadowInstance(geometry, viewId
							, &idb
							, currentShadowMapSettings->m_progPackInstance->GetProgram()
							, FRenderState::_renderState[renderStateIndex]
						);

					}

					// Draw scene.
					{
						// Setup shadow mtx.
						float mtxShadow[16];

						const float ymul = (FShadowPipline::s_flipV) ? 0.5f : -0.5f;
						float zadd = (EDepthImpl::Linear == FShadowPipline::_shadowSceneSettings.m_depthImpl) ? 0.0f : 0.5f;

						const float mtxBias[16] =
						{
							0.5f, 0.0f, 0.0f, 0.0f,
							0.0f, ymul, 0.0f, 0.0f,
							0.0f, 0.0f, 0.5f, 0.0f,
							0.5f, 0.5f, zadd, 1.0f,
						};

						if (ELightType::SpotLight == FShadowPipline::_shadowSceneSettings.m_lightType)
						{
							float mtxTmp[16];
							bx::mtxMul(mtxTmp, FShadowPipline::s_lightProj[ProjType::Horizontal].Data(), mtxBias);
							bx::mtxMul(mtxShadow, FShadowPipline::s_lightView[0].Data(), mtxTmp); //FShadowRenderer::s_lightViewProjBias
						}
						else if (ELightType::PointLight == FShadowPipline::_shadowSceneSettings.m_lightType)
						{
							const float s = (FShadowPipline::s_flipV) ? 1.0f : -1.0f; //sign
							zadd = (EDepthImpl::Linear == FShadowPipline::_shadowSceneSettings.m_depthImpl) ? 0.0f : 0.5f;

							const float mtxCropBias[2][TetrahedronFaces::Count][16] =
							{
								{ // settings.m_stencilPack == false

									{ // D3D: Green, OGL: Blue
										0.25f,    0.0f, 0.0f, 0.0f,
										0.0f, s*0.25f, 0.0f, 0.0f,
										0.0f,    0.0f, 0.5f, 0.0f,
										0.25f,   0.25f, zadd, 1.0f,
									},
									{ // D3D: Yellow, OGL: Red
										0.25f,    0.0f, 0.0f, 0.0f,
										0.0f, s*0.25f, 0.0f, 0.0f,
										0.0f,    0.0f, 0.5f, 0.0f,
										0.75f,   0.25f, zadd, 1.0f,
									},
									{ // D3D: Blue, OGL: Green
										0.25f,    0.0f, 0.0f, 0.0f,
										0.0f, s*0.25f, 0.0f, 0.0f,
										0.0f,    0.0f, 0.5f, 0.0f,
										0.25f,   0.75f, zadd, 1.0f,
									},
									{ // D3D: Red, OGL: Yellow
										0.25f,    0.0f, 0.0f, 0.0f,
										0.0f, s*0.25f, 0.0f, 0.0f,
										0.0f,    0.0f, 0.5f, 0.0f,
										0.75f,   0.75f, zadd, 1.0f,
									},
								},
								{ // settings.m_stencilPack == true

									{ // D3D: Red, OGL: Blue
										0.25f,   0.0f, 0.0f, 0.0f,
										0.0f, s*0.5f, 0.0f, 0.0f,
										0.0f,   0.0f, 0.5f, 0.0f,
										0.25f,   0.5f, zadd, 1.0f,
									},
									{ // D3D: Blue, OGL: Red
										0.25f,   0.0f, 0.0f, 0.0f,
										0.0f, s*0.5f, 0.0f, 0.0f,
										0.0f,   0.0f, 0.5f, 0.0f,
										0.75f,   0.5f, zadd, 1.0f,
									},
									{ // D3D: Green, OGL: Green
										0.5f,    0.0f, 0.0f, 0.0f,
										0.0f, s*0.25f, 0.0f, 0.0f,
										0.0f,    0.0f, 0.5f, 0.0f,
										0.5f,   0.75f, zadd, 1.0f,
									},
									{ // D3D: Yellow, OGL: Yellow
										0.5f,    0.0f, 0.0f, 0.0f,
										0.0f, s*0.25f, 0.0f, 0.0f,
										0.0f,    0.0f, 0.5f, 0.0f,
										0.5f,   0.25f, zadd, 1.0f,
									},
								}
							};

							//Use as: [stencilPack][flipV][tetrahedronFace]
							static const uint8_t cropBiasIndices[2][2][4] =
							{
								{ // settings.m_stencilPack == false
									{ 0, 1, 2, 3 }, //flipV == false
									{ 2, 3, 0, 1 }, //flipV == true
								},
								{ // settings.m_stencilPack == true
									{ 3, 2, 0, 1 }, //flipV == false
									{ 2, 3, 0, 1 }, //flipV == true
								},
							};

							for (uint8_t ii = 0; ii < TetrahedronFaces::Count; ++ii)
							{
								ProjType::Enum projType = (FShadowPipline::_shadowSceneSettings.m_stencilPack) ? ProjType::Enum(ii > 1) : ProjType::Horizontal;
								uint8_t biasIndex = cropBiasIndices[FShadowPipline::_shadowSceneSettings.m_stencilPack][uint8_t(FShadowPipline::s_flipV)][ii];

								float mtxTmp[16];
								bx::mtxMul(mtxTmp, FShadowPipline::s_mtxYpr[ii].Data(), FShadowPipline::s_lightProj[projType].Data());
								bx::mtxMul(FShadowPipline::_shadowMapMtx[ii].Data(), mtxTmp, mtxCropBias[FShadowPipline::_shadowSceneSettings.m_stencilPack][biasIndex]); //FShadowRenderer::s_mtxYprProjBias
							}

							bx::mtxTranslate(mtxShadow //lightInvTranslate
								, -lightComponent->m_position._x
								, -lightComponent->m_position._y
								, -lightComponent->m_position._z
							);
						}
						else //LightType::DirectionalLight == settings.m_lightType
						{
							for (uint8_t ii = 0; ii < FShadowPipline::_shadowSceneSettings.m_numSplits; ++ii)
							{
								float mtxTmp[16];

								bx::mtxMul(mtxTmp, FShadowPipline::s_lightProj[ii].Data(), mtxBias);
								bx::mtxMul(FShadowPipline::_shadowMapMtx[ii].Data(), FShadowPipline::s_lightView[0].Data(), mtxTmp); //lViewProjCropBias
							}
						}

						// Cube.
						if (ELightType::DirectionalLight != FShadowPipline::_shadowSceneSettings.m_lightType)
						{
							//bx::mtxMul(FShadowRenderer::_lightMtx.Data(), modelMatrix.Data(), mtxShadow);
							FShadowPipline::_lightMtx = TMatrix4x4F(mtxShadow);
						}

						SubmitShadowInstance(geometry, RENDERVIEW_DRAWSCENE_0_ID
							, &idb
							, material->GetShaderInstanceProgram().GetProgram()//currentShadowMapSettings->m_progDraw
							, FRenderState::_renderState[FRenderState::Default]
							, true
						);
					}

					//  Occlusion query pipeline
					{

						SubmitOcclusionInstace(geometry, RENDER_OCCLUSION_PASS_ID
							, &idb
							, material->GetShaderInstanceProgram().GetProgram()//currentShadowMapSettings->m_progDraw
							, FRenderState::_renderState[FRenderState::Occlusion]
							, true
						);
					}
					/*switch (bgfx::getResult(geometry->_occlusionQuery))
					{
					case bgfx::OcclusionQueryResult::Invisible:
						_invisibleBatch++; break;
					case bgfx::OcclusionQueryResult::Visible:
						_visibleBatch++; break;
					default:
						break;
					}
					*/
					batchesAddCount = instanceCount;
				}
				else
				{
					FGeometry* geometry = batch._pass._geometry;
					OMaterial* material = batch._pass._material;
					TMatrix4x4F& modelMatrix = batch._pass._worldMatrix->ToMatrix4().Transpose();
					FShadowMapSettings* currentShadowMapSettings = &FShadowPipline::_shadowMapSettings[FShadowPipline::_shadowSceneSettings.m_lightType][FShadowPipline::_shadowSceneSettings.m_depthImpl][FShadowPipline::_shadowSceneSettings.m_smImpl];
					AttachShader(batch._pass);

					uint8_t drawNum;
					if (ELightType::SpotLight == FShadowPipline::_shadowSceneSettings.m_lightType)
					{
						drawNum = 1;
					}
					else if (ELightType::PointLight == FShadowPipline::_shadowSceneSettings.m_lightType)
					{
						drawNum = 4;
					}
					else //LightType::DirectionalLight == settings.m_lightType)
					{
						drawNum = uint8_t(FShadowPipline::_shadowSceneSettings.m_numSplits);
					}

					for (uint8_t ii = 0; ii < drawNum; ++ii)
					{
						const uint8_t viewId = RENDERVIEW_SHADOWMAP_1_ID + ii;

						uint8_t renderStateIndex = FRenderState::ShadowMap_PackDepth;
						if (ELightType::PointLight == FShadowPipline::_shadowSceneSettings.m_lightType && FShadowPipline::_shadowSceneSettings.m_stencilPack)
						{
							renderStateIndex = uint8_t((ii < 2) ? FRenderState::ShadowMap_PackDepthHoriz : FRenderState::ShadowMap_PackDepthVert);
						}

						// Shadow pack.
						SubmitShadow(geometry, viewId
							, modelMatrix.Data()
							, currentShadowMapSettings->m_progPack->GetProgram()
							, FRenderState::_renderState[renderStateIndex]
						);

					}

					// Draw scene.
					{
						// Setup shadow mtx.
						float mtxShadow[16];

						const float ymul = (FShadowPipline::s_flipV) ? 0.5f : -0.5f;
						float zadd = (EDepthImpl::Linear == FShadowPipline::_shadowSceneSettings.m_depthImpl) ? 0.0f : 0.5f;

						const float mtxBias[16] =
						{
							0.5f, 0.0f, 0.0f, 0.0f,
							0.0f, ymul, 0.0f, 0.0f,
							0.0f, 0.0f, 0.5f, 0.0f,
							0.5f, 0.5f, zadd, 1.0f,
						};

						if (ELightType::SpotLight == FShadowPipline::_shadowSceneSettings.m_lightType)
						{
							float mtxTmp[16];
							bx::mtxMul(mtxTmp, FShadowPipline::s_lightProj[ProjType::Horizontal].Data(), mtxBias);
							bx::mtxMul(mtxShadow, FShadowPipline::s_lightView[0].Data(), mtxTmp); //FShadowRenderer::s_lightViewProjBias
						}
						else if (ELightType::PointLight == FShadowPipline::_shadowSceneSettings.m_lightType)
						{
							const float s = (FShadowPipline::s_flipV) ? 1.0f : -1.0f; //sign
							zadd = (EDepthImpl::Linear == FShadowPipline::_shadowSceneSettings.m_depthImpl) ? 0.0f : 0.5f;

							const float mtxCropBias[2][TetrahedronFaces::Count][16] =
							{
								{ // settings.m_stencilPack == false

									{ // D3D: Green, OGL: Blue
										0.25f,    0.0f, 0.0f, 0.0f,
										0.0f, s*0.25f, 0.0f, 0.0f,
										0.0f,    0.0f, 0.5f, 0.0f,
										0.25f,   0.25f, zadd, 1.0f,
									},
									{ // D3D: Yellow, OGL: Red
										0.25f,    0.0f, 0.0f, 0.0f,
										0.0f, s*0.25f, 0.0f, 0.0f,
										0.0f,    0.0f, 0.5f, 0.0f,
										0.75f,   0.25f, zadd, 1.0f,
									},
									{ // D3D: Blue, OGL: Green
										0.25f,    0.0f, 0.0f, 0.0f,
										0.0f, s*0.25f, 0.0f, 0.0f,
										0.0f,    0.0f, 0.5f, 0.0f,
										0.25f,   0.75f, zadd, 1.0f,
									},
									{ // D3D: Red, OGL: Yellow
										0.25f,    0.0f, 0.0f, 0.0f,
										0.0f, s*0.25f, 0.0f, 0.0f,
										0.0f,    0.0f, 0.5f, 0.0f,
										0.75f,   0.75f, zadd, 1.0f,
									},
								},
								{ // settings.m_stencilPack == true

									{ // D3D: Red, OGL: Blue
										0.25f,   0.0f, 0.0f, 0.0f,
										0.0f, s*0.5f, 0.0f, 0.0f,
										0.0f,   0.0f, 0.5f, 0.0f,
										0.25f,   0.5f, zadd, 1.0f,
									},
									{ // D3D: Blue, OGL: Red
										0.25f,   0.0f, 0.0f, 0.0f,
										0.0f, s*0.5f, 0.0f, 0.0f,
										0.0f,   0.0f, 0.5f, 0.0f,
										0.75f,   0.5f, zadd, 1.0f,
									},
									{ // D3D: Green, OGL: Green
										0.5f,    0.0f, 0.0f, 0.0f,
										0.0f, s*0.25f, 0.0f, 0.0f,
										0.0f,    0.0f, 0.5f, 0.0f,
										0.5f,   0.75f, zadd, 1.0f,
									},
									{ // D3D: Yellow, OGL: Yellow
										0.5f,    0.0f, 0.0f, 0.0f,
										0.0f, s*0.25f, 0.0f, 0.0f,
										0.0f,    0.0f, 0.5f, 0.0f,
										0.5f,   0.25f, zadd, 1.0f,
									},
								}
							};

							//Use as: [stencilPack][flipV][tetrahedronFace]
							static const uint8_t cropBiasIndices[2][2][4] =
							{
								{ // settings.m_stencilPack == false
									{ 0, 1, 2, 3 }, //flipV == false
									{ 2, 3, 0, 1 }, //flipV == true
								},
								{ // settings.m_stencilPack == true
									{ 3, 2, 0, 1 }, //flipV == false
									{ 2, 3, 0, 1 }, //flipV == true
								},
							};

							for (uint8_t ii = 0; ii < TetrahedronFaces::Count; ++ii)
							{
								ProjType::Enum projType = (FShadowPipline::_shadowSceneSettings.m_stencilPack) ? ProjType::Enum(ii > 1) : ProjType::Horizontal;
								uint8_t biasIndex = cropBiasIndices[FShadowPipline::_shadowSceneSettings.m_stencilPack][uint8_t(FShadowPipline::s_flipV)][ii];

								float mtxTmp[16];
								bx::mtxMul(mtxTmp, FShadowPipline::s_mtxYpr[ii].Data(), FShadowPipline::s_lightProj[projType].Data());
								bx::mtxMul(FShadowPipline::_shadowMapMtx[ii].Data(), mtxTmp, mtxCropBias[FShadowPipline::_shadowSceneSettings.m_stencilPack][biasIndex]); //FShadowRenderer::s_mtxYprProjBias
							}

							bx::mtxTranslate(mtxShadow //lightInvTranslate
								, -lightComponent->m_position._x
								, -lightComponent->m_position._y
								, -lightComponent->m_position._z
							);
						}
						else //LightType::DirectionalLight == settings.m_lightType
						{
							for (uint8_t ii = 0; ii < FShadowPipline::_shadowSceneSettings.m_numSplits; ++ii)
							{
								float mtxTmp[16];

								bx::mtxMul(mtxTmp, FShadowPipline::s_lightProj[ii].Data(), mtxBias);
								bx::mtxMul(FShadowPipline::_shadowMapMtx[ii].Data(), FShadowPipline::s_lightView[0].Data(), mtxTmp); //lViewProjCropBias
							}
						}

						// Cube.
						if (ELightType::DirectionalLight != FShadowPipline::_shadowSceneSettings.m_lightType)
						{
							//bx::mtxMul(FShadowRenderer::_lightMtx.Data(), modelMatrix.Data(), mtxShadow);
							FShadowPipline::_lightMtx = TMatrix4x4F(mtxShadow);
						}

						SubmitShadow(geometry, RENDERVIEW_DRAWSCENE_0_ID
							, modelMatrix.Data()
							, material->GetShaderProgram().GetProgram()//currentShadowMapSettings->m_progDraw
							, FRenderState::_renderState[FRenderState::Default]
							, true
						);

					}

					//  Occlusion query pipeline
					{

						SubmitOcclusion(geometry, RENDER_OCCLUSION_PASS_ID
							, modelMatrix.Data()
							, material->GetShaderProgram().GetProgram()//currentShadowMapSettings->m_progDraw
							, FRenderState::_renderState[FRenderState::Occlusion]
							, true
						);
					}
					/*switch (bgfx::getResult(geometry->_occlusionQuery))
					{
					case bgfx::OcclusionQueryResult::Invisible:
						_invisibleBatch++; break;
					case bgfx::OcclusionQueryResult::Visible:
						_visibleBatch++; break;
					default:
						break;
					}*/
					batchesAddCount = 1;
				}
				bIt += batchesAddCount;
			}
		}
	}
}


void FForwardShadingRenderer::ShutDowm()
{
	bgfx::shutdown();
}


void FForwardShadingRenderer::CollectActors(AWorld* world, ACameraComponent* camera)
{
	THashMap<unsigned, AActor*>& worldActors = world->GetActors();

	for (auto it = worldActors.Begin(); it != worldActors.End(); ++it)
	{
		AActor* actor = it->_second;
		unsigned short flags = actor->Flags();

		if (!(flags & ACTOR_FLAG_ENABLED))
			continue;

		//Collect geometries
		if ((flags & ACTOR_FLAG_GEOMETRY) && (actor->GetLayerMask() & camera->GetViewMask()))
		{
			_geometriesActor.Push(actor);
		}

		//Collect light
		if(flags & ACTOR_FLAG_LIGHT)
			_lightActor.Push(actor);
	}

	
	for (auto it = _lightActor.Begin(); it != _lightActor.End();++it)
	{
		TVector<ALightComponent*>& lightComponents = (*it)->GetLightComponents();
		for (auto lIt = lightComponents.Begin(); lIt != lightComponents.End(); ++lIt)
		{
			ALightComponent* lightComponent = *lIt;

			for (auto gIt = _geometriesActor.Begin(); gIt != _geometriesActor.End(); ++gIt)
			{
				AActor* actor = *gIt;
				TVector<AGeometryComponent*>& geometrys = actor->GetGeometryComponents();
				for (auto git = geometrys.Begin(); git != geometrys.End(); ++git)
				{
					AGeometryComponent* comp = *git;
					if (!comp->GetPass().isValid())
						continue;
					// Default material automatically matches shader according to the scene
					if (comp->GetPass()._material->IsDefault())
					{
						OMaterial* material = new OMaterial;
						comp->GetPass()._material = material;
					
						material->SetShaderType(EMaterialShaderType::SHADOW);
						
					}
				}
			}

		}
	}

	
}

void FForwardShadingRenderer::CollectBatch()
{
	// Loop through geometry nodes
	for (auto gIt = _geometriesActor.Begin(); gIt != _geometriesActor.End(); ++gIt)
	{
		AActor* actor = *gIt;
		TVector<AGeometryComponent*> geometryComponents = actor->GetGeometryComponents();
		if (!geometryComponents.Size())
		{
			actor->SetFlag(ACTOR_FLAG_GEOMETRY, false);
			continue;
		}

		for (auto it = geometryComponents.Begin(); it != geometryComponents.End(); ++it)
		{
			AGeometryComponent* geometryComponent = *it;
			FPass& geometryPass = geometryComponent->GetPass();
			// If the pass message is incomplete, discard it
			if (!geometryPass.isValid())
				continue;

			geometryPass.CalculateSortKey();

			FBatch newBatch;
			newBatch._pass = geometryPass;

			_batchQueues._batches.Push(newBatch);
		}
	}

	_batchQueues.Sort();
}

void FForwardShadingRenderer::AttachShader(FPass& pass)
{
	FShadowMapSettings* currentSmSettings = FShadowPipline::GetCurrentShadowMapSettings();
	OMaterial* material = pass._material;
	
	switch (material->GetShaderType())
	{
	case EMaterialShaderType::DEFAULT:
		break;
	case EMaterialShaderType::SHADOW:
		material->GetShaderProgram() = *currentSmSettings->m_progDraw;
		material->GetShaderInstanceProgram() = *currentSmSettings->m_progDrawInstace;
		break;
	default:
		break;
	}
}

void FForwardShadingRenderer::PrepareView()
{
	_batchQueues.Clear();
	_lightActor.Clear();
	_geometriesActor.Clear();
	_invisibleBatch = 0;
	_visibleBatch = 0;
	//bgfx::setViewRect(RENDER_SCENE_PASS_ID, 0, 0, uint16_t(_backbufferSize._x), uint16_t(_backbufferSize._y));
}

}