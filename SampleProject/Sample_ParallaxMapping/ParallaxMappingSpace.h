#pragma once
#include "Application.h"
using namespace Auto3D;
class ParallaxMappingSpace : public Application
{
	REGISTER_SPACE_CLASS(ParallaxMappingSpace)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};


