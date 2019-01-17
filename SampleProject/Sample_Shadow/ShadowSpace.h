#pragma once
#include "Application.h"
using namespace Auto3D;
class ShadowSpace : public Application
{
	REGISTER_SPACE_CLASS(ShadowSpace)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};


