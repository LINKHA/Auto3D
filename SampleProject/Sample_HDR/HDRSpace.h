#pragma once
#include "MotionSpace.h"

using namespace Auto3D;

class HDRSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(HDRSpace)
public:
	void Init()override;
	void Destruct()override;
};


