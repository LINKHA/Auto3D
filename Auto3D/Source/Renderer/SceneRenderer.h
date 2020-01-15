#pragma once
#include "AutoConfig.h"

namespace Auto3D
{
#define 
class AUTO_API ISceneRenderer
{
public:
	ISceneRenderer() {}
	virtual ~ISceneRenderer() {}
	
	virtual void Init(int32_t argc, const char* const* argv, uint32_t width, uint32_t height) = 0;

	// FSceneRenderer interface
	virtual void Render() = 0;
};

}