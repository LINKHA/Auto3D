#pragma once
#include "Application.h"

using namespace Auto3D;

class BloomSpace : public Application
{
	REGISTER_SPACE_CLASS(BloomSpace)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};
