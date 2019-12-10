#pragma once
#include "../Sample.h"

using namespace Auto3D;

class SerializeSample : public Sample
{
	DECLARE_CLASS(SerializeSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	float yaw = 0.0f, pitch = 20.0f;
	ACamera* camera;
	TSharedPtr<AScene> scene;
};