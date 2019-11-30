#pragma once
#include "../Sample.h"

using namespace Auto3D;

class WaterSample : public Sample
{
	REGISTER_OBJECT_CLASS(WaterSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void UIDraw()override;
	void Stop()override;
	float yaw = 0.0f, pitch = 20.0f;
	ACamera* camera;
	TSharedPtr<AScene> scene;
};