#pragma once
#include "AutoConfig.h"
#include <bgfx/bgfx.h>
#include <stdint.h>

namespace Auto3D
{
struct AUTO_API FArgs
{
	FArgs(int argc, const char* const* argv);

	bgfx::RendererType::Enum _type;
	uint16_t _pciId;
};

}