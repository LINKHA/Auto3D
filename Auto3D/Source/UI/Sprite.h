#pragma once
#include "../Scene/SpatialNode.h"

namespace Auto3D 
{

class AUTO_API Sprite : public SpatialNode
{
	REGISTER_OBJECT_CLASS(Sprite, SpatialNode)

public:
	Sprite();
	~Sprite();

};

}