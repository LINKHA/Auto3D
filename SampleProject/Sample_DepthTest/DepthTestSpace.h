#pragma once
#include "Application.h"
using namespace Auto3D;
class DepthTestSpace : public Application
{
	REGISTER_SPACE_CLASS(DepthTestSpace)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};

