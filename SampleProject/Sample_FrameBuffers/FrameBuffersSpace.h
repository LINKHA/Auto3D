#pragma once
#include "MotionSpace.h"

using namespace Auto3D;
class FreeCamera;
class FrameBuffersSpace : public MotionSpace
{
public:
	explicit FrameBuffersSpace(Ambient* ambient);
	~FrameBuffersSpace();
	void Start()override;
	void Update()override;
	int Launch();
private:
	int oldi = 0;
	FreeCamera* camera;
};

