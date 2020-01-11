#pragma once
#include "../Sample.h"

using namespace Auto3D;

class HelloWorldSample : public Sample
{
	DECLARE_CLASS(HelloWorldSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};