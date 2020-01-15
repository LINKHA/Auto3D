#include "Platform/Args.h"
#include <bx/commandline.h>

namespace Auto3D
{
FArgs::FArgs(int argc, const char* const* argv)
	: _type(bgfx::RendererType::Count)
	, _pciId(BGFX_PCI_ID_NONE)
{
	bx::CommandLine cmdLine(argc, (const char**)argv);

	if (cmdLine.hasArg("gl"))
	{
		_type = bgfx::RendererType::OpenGL;
	}
	else if (cmdLine.hasArg("vk"))
	{
		_type = bgfx::RendererType::Vulkan;
	}
	else if (cmdLine.hasArg("noop"))
	{
		_type = bgfx::RendererType::Noop;
	}
	else if (BX_ENABLED(BX_PLATFORM_WINDOWS | BX_PLATFORM_WINRT | BX_PLATFORM_XBOXONE))
	{
		if (cmdLine.hasArg("d3d9"))
		{
			_type = bgfx::RendererType::Direct3D9;
		}
		else if (cmdLine.hasArg("d3d11"))
		{
			_type = bgfx::RendererType::Direct3D11;
		}
		else if (cmdLine.hasArg("d3d12"))
		{
			_type = bgfx::RendererType::Direct3D12;
		}
	}
	else if (BX_ENABLED(BX_PLATFORM_OSX))
	{
		if (cmdLine.hasArg("mtl"))
		{
			_type = bgfx::RendererType::Metal;
		}
	}

	if (cmdLine.hasArg("amd"))
	{
		_pciId = BGFX_PCI_ID_AMD;
	}
	else if (cmdLine.hasArg("nvidia"))
	{
		_pciId = BGFX_PCI_ID_NVIDIA;
	}
	else if (cmdLine.hasArg("intel"))
	{
		_pciId = BGFX_PCI_ID_INTEL;
	}
	else if (cmdLine.hasArg("sw"))
	{
		_pciId = BGFX_PCI_ID_SOFTWARE_RASTERIZER;
	}
}

}