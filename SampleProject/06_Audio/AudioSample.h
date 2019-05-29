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

	SharedPtr<Scene> scene;
	AudioListener* listener;
	AudioSource* source1;
	AudioSource* source2;


	SharedPtr<Canvas> canvas;
	Button* button;
	Button* button2;
	Button* button3;
	Button* button4;

	SharedPtr<Canvas> canvas2;
	Button* button5;
	Button* button6;
	Button* button7;
	Button* button8;
};