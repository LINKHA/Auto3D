#pragma once
#include "MotionSpace.h"
using namespace Auto3D;

class MassInstantiationSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(MassInstantiationSpace)
public:
	void Init()override;
	void Destruct()override;
};

