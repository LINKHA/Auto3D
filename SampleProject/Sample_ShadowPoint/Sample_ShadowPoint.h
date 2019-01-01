#pragma once
#include "MotionSpace.h"
using namespace Auto3D;
class Sample_ShadowPoint : public MotionSpace
{
	REGISTER_SPACE_CLASS(Sample_ShadowPoint)
public:
	void Init()override;
	void Destruct()override;
};


