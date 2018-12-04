#pragma once
#include "MotionSpace.h"
using namespace Auto3D;
class ParallaxMappingSpace : public MotionSpace
{
public:
	explicit ParallaxMappingSpace(Ambient* ambient);
	~ParallaxMappingSpace();
	void Awake()override;
};


