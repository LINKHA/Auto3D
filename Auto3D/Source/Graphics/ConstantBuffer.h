#pragma once

#include "../AutoConfig.h"

#if defined (AUTO_OPENGL)
#	include "OGL/OGLConstantBuffer.h"
#elif defined (AUTO_VULKAN)
#	include "VK/VKConstantBuffer.h"
#elif defined (AUTO_DIRECT3D_12) 
#	include "D3D12/D3D12ConstantBuffer.h"
#endif