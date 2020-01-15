#pragma once
#include "AutoConfig.h"
#include "Container/Singleton.h"

#include <bgfx/bgfx.h>
#include <stdint.h>

namespace Auto3D
{
struct AUTO_API FArgs
{
	REGISTER_SINGLETON(FArgs);

	FArgs();
	void Init(int argc, char** argv);

	bgfx::RendererType::Enum _type;
	uint16_t _pciId;

	int _argc;
	char** _argv;
};

}