#pragma once
#include "MotionSpace.h"

using namespace Auto3D;

class TimeSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(TimeSpace)
public:
	void Init()override;
	void Destruct()override;
};