#pragma once

#include "../AutoConfig.h"

#if defined (AUTO_OPENGL)
#	include "OGL/OGLShaderVariation.h"
#elif defined (AUTO_VULKAN)
#	include "VK/VKShaderVariation.h"
#elif defined (AUTO_DIRECT3D_12) 
#	include "D3D12/D3D12ShaderVariation.h"
#endif