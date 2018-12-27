#pragma once
#include "MotionSpace.h"
	
using namespace Auto3D;
class StencilTestSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(StencilTestSpace)
public:
	void Init()override;
	void Destruct()override;
};	
