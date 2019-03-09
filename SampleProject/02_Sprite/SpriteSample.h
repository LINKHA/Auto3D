#pragma once

#include "../Sample.h"

using namespace Auto3D;

class SpriteSample : public Sample
{
	REGISTER_OBJECT_CLASS(SpriteSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	void HandleCloseRequest(Event& /* event */)
	{
		Subsystem<Graphics>()->Close();
	}
	const int starNum = 200;
};