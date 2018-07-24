#pragma once
#include "MotionSpace.h"
USING_AUTO
class ParallaxMappingSpace : public MotionSpace
{
public:
	explicit ParallaxMappingSpace(Ambient* ambient);
	~ParallaxMappingSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


