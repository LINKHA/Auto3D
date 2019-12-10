#pragma once
#include "../Sample.h"

using namespace Auto3D;

class RttrSample : public Sample
{
	DECLARE_CLASS(RttrSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};