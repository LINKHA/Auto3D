#pragma once
#include "MotionSpace.h"
using namespace Auto3D;

class MultLightsSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(MultLightsSpace)
public:
	void Init()override;
	void Destruct()override;
};

