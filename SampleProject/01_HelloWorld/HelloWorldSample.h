#pragma once

#include "Source/Application.h"

using namespace Auto3D;

class HelloWorldSample : public Application
{
	REGISTER_OBJECT_CLASS(HelloWorldSample, Application)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;

	void CreateLogo();

	void HandleCloseRequest(Event& /* event */)
	{
		Subsystem<Graphics>()->Close();
	}
	UICamera* uiCamera;
	SharedPtr<Canvas> canvas;
};