#pragma once
#include "AutoConfig.h"
#include <stdint.h>

namespace Auto3D
{

class AUTO_API ISceneRenderer
{
public:
	ISceneRenderer() {}
	virtual ~ISceneRenderer() {}
	
	virtual void Init(uint32_t width, uint32_t height) = 0;

	// FSceneRenderer interface
	virtual void Render() = 0;
};

}