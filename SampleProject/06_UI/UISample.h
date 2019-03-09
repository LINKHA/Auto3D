#pragma once
#include "../Sample.h"

using namespace Auto3D;

class UISample : public Sample
{
	REGISTER_OBJECT_CLASS(UISample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	void HandleCloseRequest(Event& /* event */)
	{
		Subsystem<Graphics>()->Close();
	}
	float yaw = 0.0f, pitch = 20.0f;
	Camera* camera;
	SharedPtr<Scene> scene;
};