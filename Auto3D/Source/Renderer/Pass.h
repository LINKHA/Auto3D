#pragma once
#include "AutoConfig.h"
#include "Renderer/ShaderProgram.h"
#include "Container/String.h"

namespace Auto3D
{

struct AUTO_API FPass
{
	FShaderProgram _shaderProgram;
	FString _geometryName;
};

}