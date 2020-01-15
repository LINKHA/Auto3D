#include "Renderer/ForwardShadingRenderer.h"
#include "Platform/PlatformDef.h"
#include "Platform/Args.h"

namespace Auto3D
{

FForwardShadingRenderer::FForwardShadingRenderer() :
	_backbufferWidth(AUTO_DEFAULT_WIDTH),
	_backbufferHeight(AUTO_DEFAULT_HEIGHT),
	_debug(BGFX_DEBUG_NONE),
	_reset(BGFX_RESET_VSYNC),
	_backbufferColor(0, 0, 0, 1),
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

	_backbufferWidth = width;
	_backbufferHeight = height;

	_debug = BGFX_DEBUG_NONE;
	_reset = 0
		| BGFX_RESET_VSYNC
		| BGFX_RESET_MSAA_X16;

	bgfx::Init init;
	init.type = args._type;
	init.vendorId = args._pciId;
	init.resolution.width = _backbufferWidth;
	init.resolution.height = _backbufferHeight;
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