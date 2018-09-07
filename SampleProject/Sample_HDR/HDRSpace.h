#pragma once
#include "MotionSpace.h"

using namespace Auto3D;

class HDRSpace : public MotionSpace
{
public:
	explicit HDRSpace(Ambient* ambient);
	~HDRSpace();
	void Awake()override;
	int Launch();
};


