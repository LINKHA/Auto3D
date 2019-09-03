#pragma once
#include "../Sample.h"

using namespace Auto3D;

class PhysicsSample : public Sample
{
	REGISTER_OBJECT_CLASS(PhysicsSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	float yaw = 0.0f, pitch = 2.0f;
	Camera* camera;
	SharedPtr<Scene> scene;
};