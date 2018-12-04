#pragma once
#include "MotionSpace.h"
using namespace Auto3D;
class DepthTestSpace : public MotionSpace
{
public:
	explicit DepthTestSpace(Ambient* ambient);
	~DepthTestSpace();
	void Awake()override;
};

