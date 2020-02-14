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
#include "Gameplay/WorldContext.h"
#include "Gameplay/World.h"
#include "Platform/ProcessWindow.h"
#include "Math/Matrix3x4.h"
#include "Math/Matrix4x4.h"

namespace Auto3D
{
#define RENDER_SHADOW_PASS_ID 0
#define RENDER_SCENE_PASS_ID  0
#define RENDER_OCCLUSION_PASS_ID 1

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
}

void FForwardShadingRenderer::Render()
{
	PrepareView();

	// Set view 0 default viewport.
	bgfx::setViewRect(0, 0, 0, uint16_t(_backbufferSize._x), uint16_t(_backbufferSize._y));
	// This dummy draw call is here to make sure that view 0 is cleared
	// if no other draw calls are submitted to view 0.
	bgfx::touch(0);
	bgfx::touch(1);
	//bgfx::touch(2);
	//bgfx::touch(3);

	AWorld* world = FWorldContext::Get().GetActiveWorld();
	TVector<ACameraComponent*>& cameras = world->GetCameras();

	for (auto it = cameras.Begin(); it != cameras.End(); ++it)
	{
		PrepareView();
		CollectGeometries(world, *it);
		CollectBatch();

		if(_shadowMaps.Size())
			RenderShadowMaps();

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
bgfx::UniformHandle s_shadowMap;
bgfx::UniformHandle u_lightPos;
bgfx::UniformHandle u_lightMtx;
FRenderState* renderState[2];
float m_view[16];
float m_proj[16];
float m_lightPos[4];

// Define matrices.
float lightView[16];
float lightProj[16];

void FForwardShadingRenderer::RenderBatches()
{
	TVector<FBatch>& batches = _batchQueues._batches;

	for (auto it = batches.Begin(); it != batches.End();)
	{
		FBatch& batch = *it;
		bool instance = batch._type == EGeometryType::INSTANCED;
		int batchesAddCount = 0;

		uint64_t state = BGFX_STATE_MASK;
		if (BGFX_STATE_MASK == state)
		{
			state = 0
				| BGFX_STATE_WRITE_RGB
				| BGFX_STATE_WRITE_A
				| BGFX_STATE_WRITE_Z
				| BGFX_STATE_DEPTH_TEST_LEQUAL
				| BGFX_STATE_CULL_CCW
				| BGFX_STATE_MSAA
				;
		}

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
				// Ordinary pipeline
				{
					bgfx::setIndexBuffer(geometry->_indexBufferHandles[i]);
					bgfx::setVertexBuffer(0, geometry->_vertexBufferHandles[i]);
					bgfx::setCondition(geometry->_occlusionQuery, true);
					bgfx::setInstanceDataBuffer(&idb);
					bgfx::setState(state);
					bgfx::submit(RENDER_SCENE_PASS_ID, material->GetShaderInstanceProgram().GetProgram(), 0, i != geometry->_vertexBufferHandles.Size() - 1);
				}

				// occlusion query pipeline
				{
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

			for (int i = 0; i < geometry->_vertexBufferHandles.Size(); ++i)
			{
				THashMap<FString,bgfx::UniformHandle>& uniformHandles = material->GetUniforms();

				FRenderState shadowState = _shadowMaps[0]._state;

				FRenderState renderState;
				renderState._state = 0
					| BGFX_STATE_WRITE_RGB
					| BGFX_STATE_WRITE_Z
					| BGFX_STATE_DEPTH_TEST_LEQUAL
					| BGFX_STATE_CULL_CCW
					| BGFX_STATE_MSAA
				;
				renderState._program = material->GetShaderProgram().GetProgram();
				renderState._viewId = RENDER_SCENE_PASS_ID;
				renderState._numTextures = 1;

				for (auto it = uniformHandles.Begin(); it != uniformHandles.End(); ++it)
				{
					if (it->_first == UNIFORM_SAMPLER)
					{
						renderState._textures[0]._flags = UINT32_MAX;
						renderState._textures[0]._stage = 0;
						renderState._textures[0]._sampler = it->_second;
						renderState._textures[0]._texture = BGFX_INVALID_HANDLE;

						renderState._textures[0]._texture = _shadowMaps[0]._fbtextures[0];
					}
				}

				// Render.
				float mtxShadow[16];
				float lightMtx[16];
				const bgfx::Caps* caps = bgfx::getCaps();

				const float sy = caps->originBottomLeft ? 0.5f : -0.5f;
				const float sz = caps->homogeneousDepth ? 0.5f : 1.0f;
				const float tz = caps->homogeneousDepth ? 0.5f : 0.0f;
				const float mtxCrop[16] =
				{
					0.5f, 0.0f, 0.0f, 0.0f,
					0.0f,   sy, 0.0f, 0.0f,
					0.0f, 0.0f, sz,   0.0f,
					0.5f, 0.5f, tz,   1.0f,
				};

				float mtxTmp[16];
				bx::mtxMul(mtxTmp, lightProj, mtxCrop);
				bx::mtxMul(mtxShadow, lightView, mtxTmp);

				bx::mtxMul(lightMtx, modelMatrix.Data(), mtxShadow);

				// Shadow pipeline
				{
					bgfx::setTransform(modelMatrix.Data());
					//lightMtx
					bgfx::setUniform(u_lightMtx, lightMtx);

					bgfx::setVertexBuffer(0, geometry->_vertexBufferHandles[i]);
					bgfx::setIndexBuffer(geometry->_indexBufferHandles[i]);
					//bgfx::setCondition(geometry->_occlusionQuery, true);
					bgfx::setState(state);
					bgfx::submit(RENDER_SHADOW_PASS_ID, shadowState._program, 0, i != geometry->_vertexBufferHandles.Size() - 1);
				}
				
				// Ordinary pipeline
				{
					bgfx::setTransform(modelMatrix.Data());

					//u_lightPos
					bgfx::setUniform(u_lightPos, m_lightPos);
						//lightMtx
					bgfx::setUniform(u_lightMtx, lightMtx);

					bgfx::setTexture(renderState._textures[0]._stage
						, renderState._textures[0]._sampler
						, renderState._textures[0]._texture
						, renderState._textures[0]._flags
					);

					bgfx::setVertexBuffer(0, geometry->_vertexBufferHandles[i]);
					bgfx::setIndexBuffer(geometry->_indexBufferHandles[i]);
					//bgfx::setCondition(geometry->_occlusionQuery, true);
					bgfx::setState(state);
					bgfx::submit(RENDER_SCENE_PASS_ID, material->GetShaderProgram().GetProgram(), 0, i != geometry->_vertexBufferHandles.Size() - 1);
				}
				//// occlusion query pipeline
				//{
				//	bgfx::setTransform(modelMatrix.Data());
				//	bgfx::setVertexBuffer(0, geometry->_vertexBufferHandles[i]);
				//	bgfx::setIndexBuffer(geometry->_indexBufferHandles[i]);
				//	bgfx::setState(0
				//		| BGFX_STATE_DEPTH_TEST_LEQUAL
				//		| BGFX_STATE_CULL_CCW
				//	);
				//	bgfx::submit(RENDER_OCCLUSION_PASS_ID, material->GetShaderProgram().GetProgram(), geometry->_occlusionQuery);
				//}

				//switch (bgfx::getResult(geometry->_occlusionQuery))
				//{
				//case bgfx::OcclusionQueryResult::Invisible:
				//	_invisibleBatch++; break;
				//case bgfx::OcclusionQueryResult::Visible:
				//	_visibleBatch++; break;
				//default:
				//	break;
				//}
			}
			batchesAddCount = 1;
		}
		it += batchesAddCount;
	}
}

void FForwardShadingRenderer::RenderBatch(FRenderState& renderState)
{

}


void FForwardShadingRenderer::RenderShadowMaps()
{
	{
		// Set view and projection matrices.
		const bx::Vec3 at = { 0.0f,  5.0f,   0.0f };
		const bx::Vec3 eye = { 0.0f, 30.0f, -60.0f };
		bx::mtxLookAt(m_view, eye, at);

		const float aspect = float(int32_t(AUTO_DEFAULT_WIDTH)) / float(int32_t(AUTO_DEFAULT_HEIGHT));
		bx::mtxProj(m_proj, 60.0f, aspect, 0.1f, 1000.0f, bgfx::getCaps()->homogeneousDepth);
	}
	
	// Setup lights.

	m_lightPos[0] = -0.5f;
	m_lightPos[1] = -1.0f;
	m_lightPos[2] = -0.5f;
	m_lightPos[3] = 0.0f;

	bgfx::setUniform(u_lightPos, m_lightPos);


	const bx::Vec3 at = { 0.0f,  0.0f,   0.0f };
	const bx::Vec3 eye = { -m_lightPos[0], -m_lightPos[1], -m_lightPos[2] };
	bx::mtxLookAt(lightView, eye, at);

	const bgfx::Caps* caps = bgfx::getCaps();
	const float area = 30.0f;
	bx::mtxOrtho(lightProj, -area, area, -area, area, -100.0f, 100.0f, 0.0f, caps->homogeneousDepth);

	for (auto it = _shadowMaps.Begin(); it != _shadowMaps.End(); ++it)
	{
		_FShadowMap& shadowMap = *it;
		bgfx::setViewRect(RENDER_SHADOW_PASS_ID, 0, 0, shadowMap._size, shadowMap._size);
		bgfx::setViewFrameBuffer(RENDER_SHADOW_PASS_ID, shadowMap._shadowMapFrameBuffer);
		bgfx::setViewTransform(RENDER_SHADOW_PASS_ID, lightView, lightProj);

		// Clear backbuffer and shadowmap framebuffer at beginning.
		bgfx::setViewClear(RENDER_SHADOW_PASS_ID
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x303030ff, 1.0f, 0
		);

	}

}

void FForwardShadingRenderer::SetupShadowMaps(size_t num, int size)
{
	if (num < 1)
		num = 1;
	size = NextPowerOfTwo(size);
	_shadowMaps.Clear();
	OMaterial* material = GResourceModule::Get().LoadResource<OMaterial>("Material/Shadow.json");
	for (int i = 0; i < num; ++i)
	{
		_shadowMaps.Push(_FShadowMap(size));
		FRenderState& state = _shadowMaps[i]._state;
		state._state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LEQUAL
			| BGFX_STATE_CULL_CCW
			| BGFX_STATE_MSAA
			;
		state._program = material->GetShaderProgram().GetProgram();
		state._viewId = RENDER_SHADOW_PASS_ID;
		state._numTextures = 0;
	}

	u_lightPos = bgfx::createUniform("u_lightPos", bgfx::UniformType::Vec4);
	u_lightMtx = bgfx::createUniform("u_lightMtx", bgfx::UniformType::Mat4);
}

void FForwardShadingRenderer::ShutDowm()
{
	bgfx::shutdown();
}


void FForwardShadingRenderer::CollectGeometries(AWorld* world, ACameraComponent* camera)
{
	THashMap<unsigned, AActor*>& worldActors = world->GetActors();
	
	for (auto it = worldActors.Begin(); it != worldActors.End(); ++it)
	{
		AActor* actor = it->_second;
		unsigned short flags = actor->Flags();
		if ((flags & NF_ENABLED) && (flags & NF_GEOMETRY) && (actor->GetLayerMask() & camera->GetViewMask()))
		{
			_geometriesActor.Push(actor);
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
}

}