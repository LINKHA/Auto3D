#pragma once
#include "../Sample.h"
using namespace Auto3D;

class AudioSample : public Sample
{
	REGISTER_OBJECT(AudioSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	void UIDraw()override;

	TSharedPtr<AScene> scene;
	AAudioListener* listener;
	AAudioSource* source1;
	AAudioSource* source2;
};