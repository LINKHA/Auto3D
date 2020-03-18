#pragma once
#include "AutoConfig.h"
#include <stdint.h>

namespace Auto3D
{

class AUTO_API IRenderer
{
public:
	IRenderer() {}
	virtual ~IRenderer() {}
	
	virtual void Init() = 0;

	// FSceneRenderer interface
	virtual void Render() = 0;
};

}