#pragma once
#include "OctreeNode.h"

namespace Auto3D
{
class Texture;
struct ShadowView;

/// %Light types.
enum LightType
{
	LIGHT_DIRECTIONAL = 0,
	LIGHT_POINT,
	LIGHT_SPOT
};

/// Dynamic light scene node.
class Light : public OctreeNode
{
	
};

}
