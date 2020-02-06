#include "Renderer/ForwardShadingRenderer.h"
#include "Platform/PlatformDef.h"
#include "Platform/Args.h"

#include "Time/Time.h"
#include "Component/MeshComponent.h"
#include "Resource/Mesh.h"

#include "Renderer/Pass.h"
#include "Resource/Material.h"

#include "Component/Transform.h"
#include "Component/CameraComponent.h"
#include "Gameplay/WorldContext.h"
#include "Gameplay/World.h"
#include "Platform/ProcessWindow.h"
#include "Math/Matrix3x4.h"
#include "Math/Matrix4x4.h"

namespace Auto3D
{
int64_t GBox::_timeOffset;
bgfx::ProgramHandle GBox::_program;
bgfx::UniformHandle GBox::_time;

bgfx::VertexBufferHandle GBox::_vbh;
bgfx::IndexBufferHandle GBox::_ibh;

FForwardShadingRenderer::FForwardShadingRenderer() :
	_backbufferSize(TVector2F(AUTO_DEFAULT_WIDTH,AUTO_DEFAULT_HEIGHT)),
	_debug(BGFX_DEBUG_NONE),
	_reset(BGFX_RESET_VSYNC),
	_backbufferColor(0.2f, 0.2f, 0.2f, 1),
	_depth(1.0f),
	_stencil(0)
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
	bgfx::setViewClear(0
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

	AWorld* world = FWorldContext::Get().GetActiveWorld();
	TVector<ACameraComponent*>& cameras = world->GetCameras();

	for (auto it = cameras.Begin(); it != cameras.End(); ++it)
	{
		PrepareView();
		CollectGeometries(world, *it);
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

			bgfx::setViewTransform(0, transposeViewMatrix.Data(), projectionMatrix.Data());
		}

		RenderBatch();

		for (auto it = _geometriesActor.Begin(); it != _geometriesActor.End(); ++it)
		{
			AActor* actor = *it;
			ATransform* transform = actor->GetTransform();
			TMatrix4x4F modelMatrix = transform->GetWorldTransform().ToMatrix4().Transpose();

			AMeshComponent* meshComponent = actor->FindComponent<AMeshComponent>();
			if (meshComponent)
			{
				meshComponent->GetMesh()->submit(0, GBox::_program, modelMatrix.Data());
				//meshComponent->GetMesh()->submitInstance(0, GBox::_program);
			}
		}
	}
	
	// Advance to next frame. Rendering thread will be kicked to
	// process submitted rendering primitives.
	bgfx::frame();

	
}

void FForwardShadingRenderer::RenderBatch()
{
	TVector<FBatch>& batches = _batchQueues._batches;
	TVector<TMatrix3x4F>& instanceTransforms = _instanceTransforms;

	int batchesSize = batches.Size();
	int instanceSize = instanceTransforms.Size();

	for (auto it = batches.Begin(); it != batches.End(); ++it)
	{
		FBatch& batch = *it;
		bool instance = batch._type == EGeometryType::INSTANCED;

		if (instance)
		{
			int instanceStart = batch._instanceStart;
			int instanceCount = batch._instanceCount;
		}
		else
		{
			const TMatrix3x4F* worldMatrix = batch._worldMatrix;
		}
	}
}

void FForwardShadingRenderer::ShutDowm()
{
	// Shutdown bgfx.
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

		AMeshComponent* meshComponent = actor->FindComponent<AMeshComponent>();

		if (meshComponent)
		{
			FBatch newBatch;
			newBatch._pass = actor->FindComponent<AMeshComponent>()->GetPass();
			newBatch._worldMatrix = &actor->GetTransform()->GetWorldTransform();

			newBatch.CalculateSortKey();

			_batchQueues._batches.Push(newBatch);
		}
	}

	_batchQueues.Sort(_instanceTransforms);
}

void FForwardShadingRenderer::PrepareView()
{
	_batchQueues.Clear();
	_geometriesActor.Clear();
	_instanceTransforms.Clear();
}

}