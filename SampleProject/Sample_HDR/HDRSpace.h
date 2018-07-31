#pragma once
#include "MotionSpace.h"
using namespace Auto3D;
class HDRSpace : public MotionSpace
{
public:
	explicit HDRSpace(Ambient* ambient);
	~HDRSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


