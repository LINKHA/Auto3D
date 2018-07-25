#pragma once
#include "MotionSpace.h"
USING_AUTO
class HDRSpace : public MotionSpace
{
public:
	explicit HDRSpace(Ambient* ambient);
	~HDRSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


