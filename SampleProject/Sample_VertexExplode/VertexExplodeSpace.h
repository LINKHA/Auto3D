#pragma once
#include "MotionSpace.h"
using namespace Auto3D;

class VertexExplodeSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(VertexExplodeSpace)
public:
	void Init()override;
	void Destruct()override;
};
