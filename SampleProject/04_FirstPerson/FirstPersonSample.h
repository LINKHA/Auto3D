#pragma once

#include "../Sample.h"

using namespace Auto3D;

class FirstPersonSample : public Sample
{
	REGISTER_OBJECT_CLASS(FirstPersonSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	void HandleCloseRequest(Event& /* event */)
	{
		Subsystem<Graphics>()->Close();
	}
	float yaw = 0.0f, pitch = 0.0f;
	Camera* camera;
	SharedPtr<Scene> scene;

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