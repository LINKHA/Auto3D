#pragma once
#include "MotionSpace.h"
using namespace Auto3D;
class ParallaxMappingSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(ParallaxMappingSpace)
public:
	void Init()override;
	void Destruct()override;
};


