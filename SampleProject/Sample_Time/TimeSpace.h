#pragma once
#include "Application.h"

using namespace Auto3D;

class TimeSpace : public Application
{
	REGISTER_SPACE_CLASS(TimeSpace)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};