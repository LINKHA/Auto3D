#pragma once
#include "MotionSpace.h"
using namespace Auto3D;

class MoreCameraSpace : public MotionSpace
{
public:
	REGISTER_SPACE_CLASS(MoreCameraSpace)
public:
	void Init()override;
	void Destruct()override;
};