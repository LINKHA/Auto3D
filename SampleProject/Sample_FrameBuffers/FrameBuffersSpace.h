#pragma once
#include "MotionSpace.h"

USING_AUTO
class FrameBuffersSpace : public MotionSpace
{
public:
	explicit FrameBuffersSpace(Ambient* ambient);
	~FrameBuffersSpace();
	void Start()override;
	void Update()override;
	int Launch();
};

