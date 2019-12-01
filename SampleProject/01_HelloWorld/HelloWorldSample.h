#pragma once
#include "../Sample.h"

using namespace Auto3D;

class HelloWorldSample : public Sample
{
	REGISTER_OBJECT(HelloWorldSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};