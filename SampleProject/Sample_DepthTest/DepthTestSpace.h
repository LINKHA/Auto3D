#pragma once
#include "MotionSpace.h"
using namespace Auto3D;
class DepthTestSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(DepthTestSpace)
public:
	void Init()override;
	void Destruct()override;
};

