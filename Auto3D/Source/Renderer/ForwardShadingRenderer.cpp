#include "Renderer/ForwardShadingRenderer.h"
#include "Platform/PlatformDef.h"
#include "Platform/Args.h"

#include "Time/Time.h"
#include "Component/MeshComponent.h"
#include <bx/timer.h>
#include "Component/CameraComponent.h"
#include "Gameplay/WorldContext.h"
#include "Gameplay/World.h"
#include "Platform/ProcessWindow.h"


namespace Auto3D
{
int64_t GBox::_timeOffset;
Mesh* GBox::_mesh;
bgfx::ProgramHandle GBox::_program;
bgfx::UniformHandle GBox::_time;
MouseState GBox::_mouseState;

FForwardShadingRenderer::FForwardShadingRenderer() :
	_backbufferSize(TVector2F(AUTO_DEFAULT_WIDTH,AUTO_DEFAULT_HEIGHT)),
	_debug(BGFX_DEBUG_NONE),
	_reset(BGFX_RESET_VSYNC),
	_backbufferColor(0.4f, 0.4f, 0.4f, 1),
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

	//cameraCreate();
	//cameraSetPosition({ 0.0f, 1.0f, -2.5f });
	//cameraSetVerticalAngle(-0.3f);

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
		ACameraComponent* camera = dynamic_cast<ACameraComponent*>(*it);

		float time = (float)((bx::getHPCounter() - GBox::_timeOffset) / double(bx::getHPFrequency()));
		bgfx::setUniform(GBox::_time, &time);

		// Set up matrices for gbuffer
		float view[16];
		camera->GetViewMtx(view);

		float proj[16];
		bx::mtxProj(proj, 60.0f, float(_backbufferSize._x) / float(_backbufferSize._y), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);


		float mtx[16];
		bx::mtxRotateXY(mtx
			, 0.0f
			, time*0.37f
		);

		meshSubmit(GBox::_mesh, 0, GBox::_program, mtx);
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

}