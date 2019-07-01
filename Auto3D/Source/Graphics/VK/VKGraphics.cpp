#include "../../Debug/Log.h"
#include "../../Debug/Profiler.h"
#include "VKGraphicsContext.h"
#include "../../Window/Window.h"
#include "../GPUObject.h"
#include "../Shader.h"
#include "VKGraphics.h"
#include "VKConstantBuffer.h"
#include "VKIndexBuffer.h"
#include "VKShaderProgram.h"
#include "VKShaderVariation.h"
#include "VKTexture.h"
#include "VKVertexBuffer.h"

#include <cstdlib>
#include <vulkan/vulkan.h>

#if defined(WIN32)
#	include <Windows.h>

#endif

#include "../../Debug/DebugNew.h"

namespace Auto3D
{

}
