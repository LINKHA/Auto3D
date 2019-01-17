#pragma once
#include "Application.h"
using namespace Auto3D;

class MoreCameraSpace : public Application
{
public:
	REGISTER_SPACE_CLASS(MoreCameraSpace)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};