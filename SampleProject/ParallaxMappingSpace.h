#pragma once
#include "MotionSpace.h"
USING_AUTO
class ParallaxMappingSpace : public MotionSpace
{
public:
	ParallaxMappingSpace();
	~ParallaxMappingSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


