#pragma once
#include "Application.h"
using namespace Auto3D;
class ShadowPointSpace : public Application
{
	REGISTER_SPACE_CLASS(ShadowPointSpace)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};


