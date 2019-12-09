#pragma once
#include "../Sample.h"

using namespace Auto3D;

class RttrSample : public Sample
{
	REGISTER_OBJECT(RttrSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};