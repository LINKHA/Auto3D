#pragma once
#include "MotionSpace.h"

using namespace Auto3D;
class FreeCamera;
class FrameBuffersSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(FrameBuffersSpace)
public:
	void Init()override;
	void Destruct()override;
};

