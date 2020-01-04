#pragma once
#include "AutoConfig.h"
#include "Container/Ptr.h"
namespace Auto3D
{

class AUTO_API FSceneRenderer : public FRefCounted
{
public:
	FSceneRenderer() {}
	virtual ~FSceneRenderer() {}

	void RenderShadowDepthMaps();

	// FSceneRenderer interface
	//virtual void Render() {}
};

}