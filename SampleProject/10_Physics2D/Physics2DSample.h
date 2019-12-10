#pragma once
#include "../Sample.h"

using namespace Auto3D;

class Physics2DSample : public Sample
{
	DECLARE_CLASS(Physics2DSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	float yaw = 0.0f, pitch = 20.0f;
	ACamera2D* camera2d;
};