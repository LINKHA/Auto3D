#pragma once
#include "Application.h"

using namespace Auto3D;

class DeferredShadingSpace : public Application
{
	REGISTER_SPACE_CLASS(DeferredShadingSpace)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};
