#pragma once

#include "Source/Application.h"

using namespace Auto3D;

class RendererSample : public Application
{
	REGISTER_OBJECT_CLASS(RendererSample, Application)
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
	float yaw = 0.0f, pitch = 20.0f;
	Camera* camera;
	SharedPtr<Scene> scene;
	UICamera* uiCamera;
	SharedPtr<Canvas> canvas;

	typedef struct RandMSG
	{
		Light* light;
		Vector3F position;
		Vector3F color;
		float xRand;
		float yRand;
		float zRand;
	};

	Vector<RandMSG> lights;
};