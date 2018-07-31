#pragma once
#include "MotionSpace.h"
using namespace Auto3D;

class MultLightsSpace : public MotionSpace
{
public:
	explicit MultLightsSpace(Ambient* ambient);
	~MultLightsSpace();
	void Start()override;
	void Update()override;
	int Launch();
};

