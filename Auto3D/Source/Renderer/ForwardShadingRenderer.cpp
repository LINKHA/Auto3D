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

namespace Auto3D
{
#define RENDER_SHADOW_PASS_ID 20
#define RENDER_SCENE_PASS_ID  21
#define RENDER_OCCLUSION_PASS_ID 22

FForwardShadingRenderer::FForwardShadingRenderer() :
	_backbufferSize(TVector2F(AUTO_DEFAULT_WIDTH,AUTO_DEFAULT_HEIGHT)),
	_debug(BGFX_DEBUG_NONE),
	_reset(BGFX_RESET_VSYNC),
	_backbufferColor(0.2f, 0.2f, 0.2f, 1),
	_depth(1.0f),
	_stencil(0),
	_invisibleBatch(0),
	_visibleBatch(0)
{

}

FForwardShadingRenderer::~FForwardShadingRenderer()
{

}
bgfx::UniformHandle s_shadowMap;
bgfx::UniformHandle u_lightPos;
bgfx::UniformHandle u_shadowMtx;
bgfx::ProgramHandle m_progShadow;
bgfx::ProgramHandle m_progShadow_i;
bgfx::ProgramHandle m_progMesh;

void FForwardShadingRenderer::Init(uint32_t width, uint32_t height)
{
	FArgs& args = FArgs::Get();

	_backbufferSize = TVector2F(width,height);

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

	// Set views  clear state.
	bgfx::setViewClear(RENDER_SCENE_PASS_ID
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, _backbufferColor.ToUInt()
		, _depth
		, _stencil
	);

	s_shadowMap = bgfx::createUniform("s_shadowMap", bgfx::UniformType::Sampler);
	u_lightPos = bgfx::createUniform("u_lightPos", bgfx::UniformType::Vec4);
	u_shadowMtx = bgfx::createUniform("u_shadowMtx", bgfx::UniformType::Mat4);
	m_progShadow = loadProgram("vs_sms_shadow", "fs_sms_shadow");
	m_progShadow_i = loadProgram("vs_sms_shadow_i", "fs_sms_shadow");
	m_progMesh = loadProgram("vs_sms_mesh", "fs_sms_mesh");
}

void FForwardShadingRenderer::Render()
{
	PrepareView();
	// This dummy draw call is here to make sure that view 0 is cleared
	// if no other draw calls are submitted to view 0.
	bgfx::touch(RENDER_SHADOW_PASS_ID);

	AWorld* world = FWorldContext::Get().GetActiveWorld();
	TVector<ACameraComponent*>& cameras = world->GetCameras();

	for (auto it = cameras.Begin(); it != cameras.End(); ++it)
	{
		PrepareView();
		CollectActors(world, *it);
		CollectBatch();

		ACameraComponent* camera = dynamic_cast<ACameraComponent*>(*it);

		// Set the view transform each camera is set once in the view
		{	
			// Set up matrices for gbuffer
			TMatrix3x4F viewMatrix = camera->GetViewMatrix();
			// Because the original location is not unified
			TMatrix4x4F transposeViewMatrix = viewMatrix.ToMatrix4().Transpose();

			camera->SetAspectRatio(float(_backbufferSize._x) / float(_backbufferSize._y));
			TMatrix4x4F projectionMatrix = camera->GetProjectionMatrix();

			bgfx::setViewTransform(RENDER_SCENE_PASS_ID, transposeViewMatrix.Data(), projectionMatrix.Data());
			bgfx::setViewRect(RENDER_SCENE_PASS_ID, 0, 0, uint16_t(_backbufferSize._x), uint16_t(_backbufferSize._y));

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
	TVector<FBatch>& batches = _batchQueues._batches;

	for (auto it = batches.Begin(); it != batches.End();)
	{
		ALightComponent* lightComponent = _lightActor[0]->FindComponent<ALightComponent>();
		const FShadowMap& shadowMap = lightComponent->GetShadowMap();

		TMatrix4x4F& lightProj = lightComponent->GetLightProj();
		TMatrix4x4F& lightView = lightComponent->GetLightView();
		TMatrix4x4F& mtxShadow = lightComponent->GetMtxShadow();
		TVector3F lightPosition = lightComponent->GetOwner()->GetTransform()->GetWorldPosition();

		FRenderState shadowState;
		shadowState._state = 0;
		shadowState._program = m_progShadow;
		shadowState._viewId = RENDER_SHADOW_PASS_ID;
		shadowState._numTextures = 0;
		shadowState._state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LEQUAL
			| BGFX_STATE_CULL_CCW
			| BGFX_STATE_MSAA
			;

		FRenderState sceneState;
		sceneState._state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LEQUAL
			| BGFX_STATE_CULL_CCW
			| BGFX_STATE_MSAA
			;
		sceneState._program = BGFX_INVALID_HANDLE;
		sceneState._viewId = RENDER_SCENE_PASS_ID;
		sceneState._numTextures = 1;
		sceneState._textures[0]._flags = UINT32_MAX;
		sceneState._textures[0]._stage = 0;
		sceneState._textures[0]._sampler = s_shadowMap;
		sceneState._textures[0]._texture = shadowMap._fbtexture;;

		bgfx::setViewRect(RENDER_SHADOW_PASS_ID, 0, 0, shadowMap._size, shadowMap._size);
		bgfx::setViewFrameBuffer(RENDER_SHADOW_PASS_ID, shadowMap._shadowMapFrameBuffer);
		bgfx::setViewTransform(RENDER_SHADOW_PASS_ID, lightView.Data(), lightProj.Data());

		// Clear backbuffer and shadowmap framebuffer at beginning.
		bgfx::setViewClear(RENDER_SHADOW_PASS_ID
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x303030ff, 1.0f, 0
		);

		bgfx::setViewClear(RENDER_SCENE_PASS_ID
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x303030ff, 1.0f, 0
		);


		FBatch& batch = *it;
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

			for (int i = 0; i < geometry->_vertexBufferHandles.Size(); ++i)
			{
				
				//// Shadow pipeline
				{
					shadowState._program = m_progShadow_i;
					bgfx::setVertexBuffer(0, geometry->_vertexBufferHandles[i]);
					bgfx::setIndexBuffer(geometry->_indexBufferHandles[i]);
					bgfx::setInstanceDataBuffer(&idb);
					bgfx::setCondition(geometry->_occlusionQuery, true);
					bgfx::setState(shadowState._state);
					bgfx::submit(shadowState._viewId, shadowState._program, 0, i != geometry->_vertexBufferHandles.Size() - 1);
				}

				// Ordinary pipeline
				{
					bgfx::setUniform(u_shadowMtx, mtxShadow.Data());
					bgfx::setUniform(u_lightPos, TVector4F(-lightPosition, 0.0f).Data());
					bgfx::setTexture(sceneState._textures[0]._stage
						, sceneState._textures[0]._sampler
						, sceneState._textures[0]._texture
						, sceneState._textures[0]._flags
					);
					bgfx::setIndexBuffer(geometry->_indexBufferHandles[i]);
					bgfx::setVertexBuffer(0, geometry->_vertexBufferHandles[i]);
					bgfx::setCondition(geometry->_occlusionQuery, true);
					bgfx::setInstanceDataBuffer(&idb);
					bgfx::setState(sceneState._state);
					bgfx::submit(sceneState._viewId, material->GetShaderInstanceProgram().GetProgram(), 0, i != geometry->_vertexBufferHandles.Size() - 1);
				}

				// occlusion query pipeline
				{
					bgfx::setUniform(u_shadowMtx, mtxShadow.Data());
					bgfx::setUniform(u_lightPos, TVector4F(-lightPosition, 0.0f).Data());
					bgfx::setTexture(sceneState._textures[0]._stage
						, sceneState._textures[0]._sampler
						, sceneState._textures[0]._texture
						, sceneState._textures[0]._flags
					);
					bgfx::setVertexBuffer(0, geometry->_vertexBufferHandles[i]);
					bgfx::setIndexBuffer(geometry->_indexBufferHandles[i]);
					bgfx::setInstanceDataBuffer(&idb);
					bgfx::setState(0
						| BGFX_STATE_DEPTH_TEST_LEQUAL
						| BGFX_STATE_CULL_CCW
					);
					bgfx::submit(RENDER_OCCLUSION_PASS_ID, material->GetShaderInstanceProgram().GetProgram(), geometry->_occlusionQuery);
				}
			

				switch (bgfx::getResult(geometry->_occlusionQuery))
				{
				case bgfx::OcclusionQueryResult::Invisible:
					_invisibleBatch++; break;
				case bgfx::OcclusionQueryResult::Visible:
					_visibleBatch++; break;
				default:
					break;
				}
			}
			batchesAddCount = instanceCount;
		}
		else
		{
			FGeometry* geometry = batch._pass._geometry;
			OMaterial* material = batch._pass._material;

			TMatrix4x4F& modelMatrix = batch._pass._worldMatrix->ToMatrix4().Transpose();
			TMatrix4x4F lightMtx;
			lightMtx = modelMatrix * mtxShadow;

			for (int i = 0; i < geometry->_vertexBufferHandles.Size(); ++i)
			{
				THashMap<FString,bgfx::UniformHandle>& uniformHandles = material->GetUniforms();
				
				//// Shadow pipeline
				{
					bgfx::setTransform(modelMatrix.Data());

					bgfx::setVertexBuffer(0, geometry->_vertexBufferHandles[i]);
					bgfx::setIndexBuffer(geometry->_indexBufferHandles[i]);
					bgfx::setCondition(geometry->_occlusionQuery, true);
					bgfx::setState(shadowState._state);
					bgfx::submit(shadowState._viewId, shadowState._program, 0, i != geometry->_vertexBufferHandles.Size() - 1);
				}
				
				// Ordinary pipeline
				{
					bgfx::setTransform(modelMatrix.Data());
					bgfx::setUniform(u_shadowMtx, mtxShadow.Data());
					bgfx::setUniform(u_lightPos, TVector4F(-lightPosition, 0.0f).Data());

					bgfx::setTexture(sceneState._textures[0]._stage
						, sceneState._textures[0]._sampler
						, sceneState._textures[0]._texture
						, sceneState._textures[0]._flags
					);

					bgfx::setVertexBuffer(0, geometry->_vertexBufferHandles[i]);
					bgfx::setIndexBuffer(geometry->_indexBufferHandles[i]);
					bgfx::setCondition(geometry->_occlusionQuery, true);
					bgfx::setState(sceneState._state);
					bgfx::submit(sceneState._viewId, material->GetShaderProgram().GetProgram(), 0, i != geometry->_vertexBufferHandles.Size() - 1);
				}

				// occlusion query pipeline
				{
					bgfx::setTransform(modelMatrix.Data());
					bgfx::setUniform(u_shadowMtx, mtxShadow.Data());
					bgfx::setUniform(u_lightPos, TVector4F(-lightPosition, 0.0f).Data());

					bgfx::setTexture(sceneState._textures[0]._stage
						, sceneState._textures[0]._sampler
						, sceneState._textures[0]._texture
						, sceneState._textures[0]._flags
					);

					bgfx::setVertexBuffer(0, geometry->_vertexBufferHandles[i]);
					bgfx::setIndexBuffer(geometry->_indexBufferHandles[i]);
					bgfx::setState(0
						| BGFX_STATE_DEPTH_TEST_LEQUAL
						| BGFX_STATE_CULL_CCW
					);
					bgfx::submit(RENDER_OCCLUSION_PASS_ID, material->GetShaderProgram().GetProgram(), geometry->_occlusionQuery);
				}

				switch (bgfx::getResult(geometry->_occlusionQuery))
				{
				case bgfx::OcclusionQueryResult::Invisible:
					_invisibleBatch++; break;
				case bgfx::OcclusionQueryResult::Visible:
					_visibleBatch++; break;
				default:
					break;
				}
			}
			batchesAddCount = 1;
		}
		it += batchesAddCount;
	}
}

void FForwardShadingRenderer::RenderBatch(FRenderState& renderState)
{

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

		if (!(flags & NF_ENABLED))
			continue;

		//Collect geometries
		if ((flags & NF_GEOMETRY) && (actor->GetLayerMask() & camera->GetViewMask()))
		{
			_geometriesActor.Push(actor);
		}

		//Collect light
		if (actor->FindComponent<ALightComponent>())
			_lightActor.Push(actor);
	}

	if (_lightActor.Size())
	{
		for (auto it = _geometriesActor.Begin(); it != _geometriesActor.End(); ++it)
		{
			AActor* actor = *it;
			TVector<AGeometryComponent*>& geometrys = actor->GetGeometryComponents();
			for (auto git = geometrys.Begin(); git != geometrys.End(); ++git)
			{
				AGeometryComponent* comp = *git;
				if(!comp->GetPass().isValid())
					continue;
				// Default material automatically matches shader according to the scene
				if (comp->GetPass()._material == OMaterial::DefaultMaterial())
				{
					//Temp: hard shadaw map
					OMaterial* material = new OMaterial;
					comp->GetPass()._material = material;
					material->GetShaderProgram().CreateVertexShader("vs_shadowmaps_color_lighting");
					material->GetShaderProgram().CreatePixelShader("fs_shadowmaps_color_lighting_hard");
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
			actor->SetFlag(NF_GEOMETRY, false);
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


void FForwardShadingRenderer::PrepareView()
{
	_batchQueues.Clear();
	_geometriesActor.Clear();
	_invisibleBatch = 0;
	_visibleBatch = 0;
	bgfx::setViewRect(RENDER_SCENE_PASS_ID, 0, 0, uint16_t(_backbufferSize._x), uint16_t(_backbufferSize._y));
}

}