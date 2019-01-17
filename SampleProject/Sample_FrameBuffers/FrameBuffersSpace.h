#pragma once
#include "Application.h"

using namespace Auto3D;
class FreeCamera;
class FrameBuffersSpace : public Application
{
	REGISTER_SPACE_CLASS(FrameBuffersSpace)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};

