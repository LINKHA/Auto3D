#pragma once
#include "AutoConfig.h"
#if AUTO_VULKAN
#include "../../Container/String.h"
#include "../GPUObject.h"
#include "../GraphicsDefs.h"
namespace Auto3D
{

class FGraphicsModule;
class FShaderVariation;
/// Linked shader program consisting of vertex and pixel shaders.
class AUTO_API FShaderProgram : public FGPUObject
{
};

}
#endif