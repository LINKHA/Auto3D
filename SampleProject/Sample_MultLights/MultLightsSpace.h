#pragma once
#include "Application.h"
using namespace Auto3D;

class MultLightsSpace : public Application
{
	REGISTER_SPACE_CLASS(MultLightsSpace)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};

