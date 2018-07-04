#pragma once
#include "MotionSpace.h"

USING_AUTO
class FrameBuffersSpace : public MotionSpace
{
public:
	FrameBuffersSpace();
	~FrameBuffersSpace();
	void Start()override;
	void Update()override;
	int Launch();
};

