#pragma once
#include "Application.h"

using namespace Auto3D;

class HDRSpace : public Application
{
	REGISTER_SPACE_CLASS(HDRSpace)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};


