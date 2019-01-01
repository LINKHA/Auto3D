#pragma once
#include "Application.h"

using namespace Auto3D;

class AntiAliasingSpace : public Application
{
	REGISTER_SPACE_CLASS(AntiAliasingSpace)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};


