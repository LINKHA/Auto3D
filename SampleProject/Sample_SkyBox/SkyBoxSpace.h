#pragma once

#include "MotionSpace.h"
#include "Launch.h"
using namespace Auto3D;

class SkyBoxSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(SkyBoxSpace)
public:
	void Init()override;
	void Destruct()override;
};
