#pragma once
#include "MotionSpace.h"
using namespace Auto3D;
class ShadowSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(ShadowSpace)
public:
	void Init()override;
	void Destruct()override;
};


