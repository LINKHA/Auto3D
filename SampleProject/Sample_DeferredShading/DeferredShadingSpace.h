#pragma once
#include "MotionSpace.h"

using namespace Auto3D;

class DeferredShadingSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(DeferredShadingSpace)
public:
	void Init()override;
	void Destruct()override;
};
