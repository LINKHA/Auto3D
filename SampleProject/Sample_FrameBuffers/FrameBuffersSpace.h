#pragma once
#include "MotionSpace.h"

using namespace Auto3D;
class FreeCamera;
class FrameBuffersSpace : public MotionSpace
{
public:
	explicit FrameBuffersSpace(Ambient* ambient);
	~FrameBuffersSpace();
	void Awake()override;
};

