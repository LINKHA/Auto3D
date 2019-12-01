#pragma once
#include "../Sample.h"

using namespace Auto3D;

class PBRSample : public Sample
{
	REGISTER_OBJECT(PBRSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	void UIDraw()override;

	float yaw = 0.0f, pitch = 20.0f;
	ACamera* camera;
	TSharedPtr<AScene> scene;
};