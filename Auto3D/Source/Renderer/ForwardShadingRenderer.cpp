#include "Renderer/ForwardShadingRenderer.h"
#include "Platform/PlatformDef.h"
#include "Platform/Args.h"

#include "Component/MeshComponent.h"
#include <bx/timer.h>
#include "Component/CameraComponent.h"

namespace Auto3D
{
int64_t GBox::_timeOffset;
Mesh* GBox::_mesh;
bgfx::ProgramHandle GBox::_program;
bgfx::UniformHandle GBox::_time;
uint32_t GBox::_width;
uint32_t GBox::_height;
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

	cameraCreate();
	cameraSetPosition({ 0.0f, 1.0f, -2.5f });
	cameraSetVerticalAngle(-0.3f);

}

void FForwardShadingRenderer::Render()
{
	// Set view 0 default viewport.
	bgfx::setViewRect(0, 0, 0, uint16_t(GBox::_width), uint16_t(GBox::_height));
	// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to view 0.
	bgfx::touch(0);

	float time = (float)((bx::getHPCounter() - GBox::_timeOffset) / double(bx::getHPFrequency()));
	bgfx::setUniform(GBox::_time, &time);

	//const bx::Vec3 at = { 0.0f, 1.0f,  0.0f };
	//const bx::Vec3 eye = { 0.0f, 1.0f, -2.5f };

	//// Set view and projection matrix for view 0.
	//{
	//	float view[16];
	//	bx::mtxLookAt(view, eye, at);

	//	float proj[16];
	//	bx::mtxProj(proj, 60.0f, float(GBox::_width) / float(GBox::_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
	//	bgfx::setViewTransform(0, view, proj);
	//}

	// Update camera
	cameraUpdate(0.016f*0.15f, GBox::_mouseState);

	// Set up matrices for gbuffer
	float view[16];
	cameraGetViewMtx(view);

	float proj[16];
	bx::mtxProj(proj, 60.0f, float(GBox::_width) / float(GBox::_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
	bgfx::setViewTransform(0, view, proj);


	float mtx[16];
	bx::mtxRotateXY(mtx
		, 0.0f
		, time*0.37f
	);

	meshSubmit(GBox::_mesh, 0, GBox::_program, mtx);

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