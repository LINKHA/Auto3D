#pragma once
#include "../Sample.h"
using namespace Auto3D;

class AudioSample : public Sample
{
	REGISTER_OBJECT_CLASS(AudioSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	void UIDraw()override;

	SharedPtr<Scene> scene;
	AudioListener* listener;
	AudioSource* source1;
	AudioSource* source2;
};