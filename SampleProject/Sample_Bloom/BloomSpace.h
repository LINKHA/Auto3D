#pragma once
#include "MotionSpace.h"

using namespace Auto3D;

class BloomSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(BloomSpace)
public:
	void Init()override;
	void Destruct()override;
};
