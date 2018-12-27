#pragma once
#include "MotionSpace.h"

using namespace Auto3D;

class AntiAliasingSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(AntiAliasingSpace)
public:
	void Init()override;
	void Destruct()override;
};


