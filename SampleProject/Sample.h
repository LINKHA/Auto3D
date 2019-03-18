#pragma once

#include "Source/Application.h"

using namespace Auto3D;

class Sample : public Application
{
	REGISTER_OBJECT_CLASS(Sample, Application)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	void CreateLogo();

	UICamera* uiCamera;
	SharedPtr<Canvas> canvas;
};

#include "Sample.inl"