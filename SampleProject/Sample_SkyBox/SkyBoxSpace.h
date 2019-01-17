#pragma once

#include "Application.h"
#include "Launch.h"
using namespace Auto3D;

class SkyBoxSpace : public Application
{
	REGISTER_SPACE_CLASS(SkyBoxSpace)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};
