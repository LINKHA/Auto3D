#include "Renderer/ForwardShadingRenderer.h"
#include "Platform/PlatformDef.h"
#include "Platform/Args.h"

#include "Time/Time.h"
#include "Component/MeshComponent.h"
#include <bx/timer.h>
#include "Resource/Mesh.h"

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

	// Set view 0 default viewport.
	bgfx::setViewRect(0, 0, 0, uint16_t(_backbufferSize._x), uint16_t(_backbufferSize._y));
	// This dummy draw call is here to make sure that view 0 is cleared
	// if no other draw calls are submitted to view 0.
	bgfx::touch(0);

	AWorld* world = FWorldContext::Get().GetActiveWorld();
	TVector<ACameraComponent*>& cameras = world->GetCameras();

	for (auto it = cameras.Begin(); it != cameras.End(); ++it)
	{
		TVector<AActor*> geometries;
		CollectGeometries(geometries, world, *it);

		ACameraComponent* camera = dynamic_cast<ACameraComponent*>(*it);

		float time = (float)((bx::getHPCounter() - GBox::_timeOffset) / double(bx::getHPFrequency()));
		bgfx::setUniform(GBox::_time, &time);

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

		for (auto it = geometries.Begin(); it != geometries.End(); ++it)
		{
			AActor* actor = *it;
			ATransform* transform = actor->GetTransform();
			TMatrix4x4F modelMatrix = transform->GetWorldTransform().ToMatrix4().Transpose();

			AMeshComponent* meshComponent = actor->FindComponent<AMeshComponent>();
			if (meshComponent)
			{
				meshComponent->GetMesh()->submit(0, GBox::_program, modelMatrix.Data());
			}
		}


		////////////////////////////////////////////////////////////////////////////
		//// 80 bytes stride = 64 bytes for 4x4 matrix + 16 bytes for RGBA color.
		//const uint16_t instanceStride = 80;
		//// 11x11 cubes
		//const uint32_t numInstances = 121;

		//if (numInstances == bgfx::getAvailInstanceDataBuffer(numInstances, instanceStride))
		//{
		//	bgfx::InstanceDataBuffer idb;
		//	bgfx::allocInstanceDataBuffer(&idb, numInstances, instanceStride);

		//	uint8_t* data = idb.data;

		//	// Write instance data for 11x11 cubes.
		//	for (uint32_t yy = 0; yy < 11; ++yy)
		//	{
		//		for (uint32_t xx = 0; xx < 11; ++xx)
		//		{
		//			float* mtx = (float*)data;
		//			bx::mtxRotateXY(mtx, time + xx * 0.21f, time + yy * 0.37f);
		//			mtx[12] = -15.0f + float(xx)*3.0f;
		//			mtx[13] = -15.0f + float(yy)*3.0f;
		//			mtx[14] = 0.0f;

		//			float* color = (float*)&data[64];
		//			color[0] = bx::sin(time + float(xx) / 11.0f)*0.5f + 0.5f;
		//			color[1] = bx::cos(time + float(yy) / 11.0f)*0.5f + 0.5f;
		//			color[2] = bx::sin(time*3.0f)*0.5f + 0.5f;
		//			color[3] = 1.0f;

		//			data += instanceStride;
		//		}
		//	}

		//	// Set vertex and index buffer.
		//	bgfx::setVertexBuffer(0, GBox::_vbh);
		//	bgfx::setIndexBuffer(GBox::_ibh);

		//	// Set instance data buffer.
		//	bgfx::setInstanceDataBuffer(&idb);

		//	// Set render states.
		//	bgfx::setState(BGFX_STATE_DEFAULT);

		//	// Submit primitive for rendering to view 0.
		//	bgfx::submit(0, GBox::_program);
		//}
		//////////////////////////////////////////////////////////////////////////

		
	}
	
	// Advance to next frame. Rendering thread will be kicked to
	// process submitted rendering primitives.
	bgfx::frame();

	
}

void FForwardShadingRenderer::ShutDowm()
{
	// Shutdown bgfx.
	bgfx::shutdown();
}	


void FForwardShadingRenderer::CollectGeometries(TVector<AActor*>& geometries,AWorld* world, ACameraComponent* camera)
{
	THashMap<unsigned, AActor*>& worldActors = world->GetActors();
	
	for (auto it = worldActors.Begin(); it != worldActors.End(); ++it)
	{
		AActor* actor = it->_second;
		unsigned short flags = actor->Flags();
		if ((flags & NF_ENABLED) && (flags & NF_GEOMETRY) && (actor->GetLayerMask() & camera->GetViewMask()))
		{
			geometries.Push(actor);
		}
	}
}

}