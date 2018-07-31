#pragma once
#include "MotionSpace.h"
using namespace Auto3D;
class ParallaxMappingSpace : public MotionSpace
{
public:
	explicit ParallaxMappingSpace(Ambient* ambient);
	~ParallaxMappingSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


